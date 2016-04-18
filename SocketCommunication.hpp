const int clientVersionNumber = 1; //Software version of the client to check against the server

//Socket
//WSADATA wsa;
int s;
struct sockaddr_in server;
char *message, server_reply[2000];
int recv_size;

//Logic
bool connected = false;
int clientID = -1;
char *pEnd;

//Multiplayer Values
int mul_numberofplayers = 2;
bool *mul_playeralive;
int mul_whosturn = 1;

void WaitForAnswer(bool &_received) {
	//Now await an answer from the server
	/*if ((recv_size = recv(s, server_reply, 2000, 0)) == SOCKET_ERROR) {
		puts("recv failed");
	}*/
	recv_size = recv(s, server_reply, 2000, 0);

	_received = true;
}

void InitializeSocketConnection(InformationDisplay *_infoptr, GameObject *_gameptr) {
	/*if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return;
	}
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
	}*/

	s = socket(AF_INET, SOCK_STREAM, 0);

	server.sin_addr.s_addr = inet_addr("178.254.39.73");
	server.sin_family = AF_INET;
	server.sin_port = htons(51717);

	//Connect to remote server
	if (connect(s, (struct sockaddr *) &server, sizeof(server)) < 0) {
		puts("connect error");
		_infoptr->SetError(0);
		_gameptr->SetState(0);
		return;
	}

	//Check if the client has the latest version
	std::string sbufstr;
	std::stringstream out;
	out << clientVersionNumber;
	sbufstr = out.str();
	message = (char *)sbufstr.c_str();
	if (send(s, message, strlen(message), 0) < 0) {
		puts("Send failed");
		return;
	}

	//Now wait for the feedback from the server
	/*if ((recv_size = recv(s, server_reply, 2000, 0)) == SOCKET_ERROR) {
		puts("recv failed");
	}*/
	recv_size = recv(s, server_reply, 2000, 0);

	//Add a NULL terminating character to make it a proper string before printing
	server_reply[recv_size] = '\0';
	cout << "Version check :" << endl;
	puts(server_reply);

	char versionnotaccepted[] = "0";
	if (!strcmp(server_reply, versionnotaccepted)) {
		cout << "Version not accepted!" << endl;
		_infoptr->SetError(6);
		_gameptr->SetState(0);
		return;
	} else {
		cout << "Connection established" << endl;
		connected = true;
	}
}

void JoinGame(bool &_gamestarted, bool &_reset, InformationDisplay *_infoptr, GameObject *_gameptr) {

	//Send "Join" to enter a lobby or to create on
	message = "join";
	if (send(s, message, strlen(message), 0) < 0) {
		puts("Send failed");
		return;
	}

	puts("Trying to join a game");

	bool answerrec = false;
	int starttime = static_cast<int> (time(NULL));
	thread tryanswer(WaitForAnswer, ref(answerrec));
	tryanswer.detach();


	while (!answerrec) {
		if (time(NULL) - starttime <= 5) {
		} else {
			cout << "After 5 seconds the client wasn´t able to connect to a lobby. Please retry to start a online game." << endl;
			_infoptr->SetError(1);
			break;
		}
	}

	if (answerrec) {
		puts("You have entered a game lobby");

		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size] = '\0';
		puts(server_reply);
		clientID = strtol(server_reply, &pEnd, 10);
		if (clientID == 0) {
			cout << "You are the first player in the lobby. You are now waiting for more players." << endl;
			_infoptr->SetToRender(2);
			//Wait for an update in the lobby
			/*if ((recv_size = recv(s, server_reply, 2000, 0)) == SOCKET_ERROR) {
				puts("recv failed");
			}*/
			recv_size = recv(s, server_reply, 2000, 0);
			//Add a NULL terminating character to make it a proper string before printing
			server_reply[recv_size] = '\0';
			puts(server_reply);
			cout << "A second player has joined your lobby. The game will start within 20 seconds." << endl;
			_infoptr->SetToRender(5);
		} else if (clientID == 1) {
			cout << "You are the second player in the lobby. The game will start within 20 seconds." << endl;
			_infoptr->SetToRender(3);
		} else if (clientID == 2) {
			cout << "You are the third player in the lobby. Your game will start in under 20 seconds." << endl;
			_infoptr->SetToRender(4);
		}

		//Now wait for the loby settings
		/*if ((recv_size = recv(s, server_reply, 2, 0)) == SOCKET_ERROR) { //Only accept 2 characters here
			puts("recv failed");
		}*/
		recv_size = recv(s, server_reply, 2, 0);

		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size] = '\0';
		puts(server_reply);

		//Change local settings
		char buffer[10];
		strncpy(buffer, server_reply, 2); //Get the first character of the reply
		int tmpnum = strtol(buffer, &pEnd, 10);
		mul_numberofplayers = tmpnum / 10;
		mul_whosturn = tmpnum % 10;

		//Now wait for the start signal
		/*if ((recv_size = recv(s, server_reply, 2000, 0)) == SOCKET_ERROR) {
			puts("recv failed");
		}*/
		recv_size = recv(s, server_reply, 2000, 0);

		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size] = '\0';
		cout << "Statusreport :" << endl;
		puts(server_reply);

		//Check if there is an error in the lobby
		char tmperr[] = "errorwithlobby";
		if (!strcmp(server_reply, tmperr)) {
			_infoptr->SetError(2);
			_gameptr->SetState(0);
		} else {
			_gamestarted = true;
		}
	} else {
		_reset = true;
	}

}

void SendMove(int _row, int _column, int _newturn) {
	int test = clientID;
	test = test * 10;
	test += _row;
	test = test * 10;
	test += _column;
	test = test * 10;
	test += _newturn + 1;

	std::string sbufstr;
	std::stringstream out;
	out << test;
	sbufstr = out.str();
	message = (char *)sbufstr.c_str();

	if (send(s, message, strlen(message), 0) < 0) {
		puts("Send failed");
		return;
	}

	cout << "I send this move: " << message << endl;
}

void GetMove(int &player, int &_row, int &_column, int &_newturn, bool &_received, InformationDisplay *_infoptr, GameObject *_gameptr) {
	cout << "This client waits for a move socket" << endl;
	bool errorcheck = true;

	while (errorcheck) {
		//Wait for the move
		/*if ((recv_size = recv(s, server_reply, 2000, 0)) == SOCKET_ERROR) {
			puts("recv failed");
		}*/
		recv_size = recv(s, server_reply, 2000, 0);

		//Add a NULL terminating character to make it a proper string before printing
		server_reply[recv_size] = '\0';
		puts("received:");
		puts(server_reply);

		char tmp[] = "starting";
		char tmplost[] = "lost";
		if (strcmp(server_reply, tmp)) { //Filter //&& recv_size != 0
			char tmp2[] = "error";
			if (!strcmp(server_reply, tmp2)) {
				//An error in the lobby occured
				cout << "Lobby closed" << endl;
				_infoptr->SetError(2);
				_gameptr->SetState(0);
				return;
			} else if (!strcmp(server_reply, tmplost)) {
				//You lost
				cout << "You lost" << endl;
				_infoptr->SetError(4);
				_gameptr->SetState(0);
				return;
			} else {
				player = server_reply[0] - 48;
				_row = server_reply[1] - 48;
				_column = server_reply[2] - 48;
				_newturn = server_reply[3] - 48;

				cout << player << _row << _column << _newturn << endl;

				_received = true;
				errorcheck = false;
				break;
			}
		} else {
			errorcheck = true;
		}
	}
}
