//============================================================================
// Name        : server.cpp
// Author      : Leon Eck
// Version     : 1.0
// Copyright   : Copyright Leon Eck 2014
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <iostream>
#include <time.h>
#include <vector>
#include <thread>

using namespace std;

void manageOneLobby(vector<int> _lobbysocks, int _maxplayers);
void acceptAConnection(int &_newsock, bool &_connected, int &_sockfd,
struct sockaddr_in &_cli_addr, socklen_t &_clilen);

void error(const char *msg) {
	perror(msg);
	exit(1);
}

char acceptedVersion[] = "1";


int numberofplayers = 0;
int whosturn = 0;

//Initialization
vector<int> holdingsock; //All new connections go here
int sockfd, newsockfd, portno;
socklen_t clilen;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;
int n;

int main(int argc, char **argv) {
	srand(time(NULL));

	char	*pEnd;
	int maxplayers = 4;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = 51717;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	while (true) {
		holdingsock.clear();
		numberofplayers = 0;

		//Filling lobby
		while (numberofplayers < 2) {  //Wait for at least two clients
			listen(sockfd, 5);
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd,
				(struct sockaddr *) &cli_addr,
				&clilen);
			if (newsockfd < 0)
				error("ERROR on accept");

			//Check Version number
			bool versionOk = false;
			bzero(buffer, 256);
			n = read(newsockfd, buffer, 255);
			if (n < 0) error("ERROR reading from socket");
			printf("Here is the message: %s\n", buffer);
			if (strcmp(buffer, acceptedVersion)) {
				write(newsockfd, "0", 1); //Version not accepted
				versionOk = false;
			} else {
				write(newsockfd, "1", 1); //Version accepted
				versionOk = true;
			}

			if (versionOk) {
				//By success procede with joining
				holdingsock.push_back(newsockfd);
				bzero(buffer, 256);
				n = read(newsockfd, buffer, 255);
				if (n < 0) error("ERROR reading from socket");
				printf("Here is the message: %s\n", buffer);
				char tmp[] = "join";
				if (!strcmp(buffer, tmp)) {
					//When the second player joins, inform the first client
					if (numberofplayers == 1) {
						n = write(holdingsock[0], "update", 6);
						if (n < 0) {
							//The first client has left the lobby
							cout << "The first player has left the lobby" << endl;
							numberofplayers = 0; //Reset the lobby
							holdingsock.erase(holdingsock.begin());
						}
					}
					std::string sbufstr;
					std::stringstream out;
					out << numberofplayers;
					sbufstr = out.str();
					char *s = (char *)sbufstr.c_str();
					n = write(newsockfd, (void *)s, 18);
					if (n < 0) error("ERROR writing to socket");
					numberofplayers++;
				}
			}
		}

		int starttime = time(NULL);

		while (time(NULL) - starttime <= 20 && numberofplayers < 4) {
			cout << "Searching for extra players" << endl;
			listen(sockfd, 5);
			clilen = sizeof(cli_addr);

			bool conn = false;
			thread connth(acceptAConnection, ref(newsockfd), ref(conn), ref(sockfd), ref(cli_addr), ref(clilen));
			connth.detach();
			while (!conn) {
				if (time(NULL) - starttime <= 20) {
					//cout << "Waiting for another player" << endl;
				} else {
					break;
				}
			}
			if (!conn) { //Still no connection
				cout << "No additional players" << endl;
				break;
			}

			if (newsockfd < 0)
				error("ERROR on accept");

			//Check Version number
			bool versionOk = false;
			bzero(buffer, 256);
			n = read(newsockfd, buffer, 255);
			if (n < 0) error("ERROR reading from socket");
			printf("Here is the message: %s\n", buffer);
			if (strcmp(buffer, acceptedVersion)) {
				write(newsockfd, "0", 1); //Version not accepted
				versionOk = false;
			} else {
				write(newsockfd, "1", 1); //Version accepted
				versionOk = true;
			}

			if (versionOk) {
				holdingsock.push_back(newsockfd);
				bzero(buffer, 256);
				n = read(newsockfd, buffer, 255);
				if (n < 0) error("ERROR reading from socket");
				printf("Here is the message: %s\n", buffer);
				char tmp[] = "join";
				if (!strcmp(buffer, tmp)) {
					std::string sbufstr;
					std::stringstream out;
					out << numberofplayers;
					sbufstr = out.str();
					char *s = (char *)sbufstr.c_str();
					n = write(newsockfd, (void *)s, 18);
					if (n < 0) error("ERROR writing to socket");
					numberofplayers++;
				}
			}
		}

		thread tmpth(manageOneLobby, holdingsock, numberofplayers);
		tmpth.detach();
	}

	close(sockfd);
	return 0;
}

void manageOneLobby(vector<int> _lobbysocks, int _maxplayers) {
	vector<int>::iterator ii;
	int whosturn = 0;

	int output = _maxplayers;
	output = output * 10;
	int randomplayer = -1;
	randomplayer = rand() % _maxplayers + 1;
	output += randomplayer;
	whosturn = randomplayer - 1;

	bool errorinlobby = false;
	vector<int> no;
	vector<int>::iterator nono;

	//Send the number of players to all clients and a random starter
	for (ii = _lobbysocks.begin(); ii != _lobbysocks.end(); ii++) {
		std::string sbufstr;
		std::stringstream out;
		out << output;
		sbufstr = out.str();
		char *s = (char *)sbufstr.c_str();
		cout << s << endl;
		n = write(*ii, (void *)s, 2);
		if (n < 0) {
			cout << "Client is out of reach" << endl;
			no.push_back(*ii);
			errorinlobby = true;
		}
	}

	//Send message that the game is starting
	puts("Sending start message");
	for (ii = _lobbysocks.begin(); ii != _lobbysocks.end(); ii++) {
		if (!errorinlobby) {
			n = write(*ii, "starting", 8);
			if (n < 0) {
				cout << "Cant write starting" << endl;
			}
		} else {
			bool problem = false;
			for (nono = no.begin(); nono != no.end(); nono++) {
				if (*ii != *nono) {
				} else {
					problem = true;
					break;
				}
			}
			if (!problem) {
				n = write(*ii, "errorwithlobby", 14);
				if (n < 0) {
					cout << "Cant write errorwithlobby" << endl;
				}
			}
		}
	}
	if (errorinlobby) {
		cout << "Closing Lobby" << endl;
		return; //Lobby can be closed because of an error
	}
	puts("All clients were sent the start signal");

	//From here on out the game has started and the server is now always receiving inputs, he than broadcasts them to all
	vector<int> disconnectv;
	vector<int>::iterator discoii;
	while (true) {
		bzero(buffer, 256);
		n = read(_lobbysocks[whosturn], buffer, 255);
		if (n < 0) {
			//One client that should be playing has disconnected
			//Send to everyone that the lobby is closed
			for (ii = _lobbysocks.begin(); ii != _lobbysocks.end(); ii++) {
				n = write(*ii, "error", 5);
			}
			return;
		}
		printf("Here is the message: %s\n", buffer);

		//If the message is "disconnect" then send all clients an error message
		char tmpdc[] = "disconnect";
		if (!strcmp(buffer, tmpdc)) {
			cout << "Received a disconnect" << endl;
			//One client that should be playing has disconnected
			//Send to everyone that the lobby is closed
			for (ii = _lobbysocks.begin(); ii != _lobbysocks.end(); ii++) {
				bool sendingtodisco = false;
				for (discoii = disconnectv.begin(); discoii != disconnectv.end(); discoii++) {
					if (*ii == *discoii) {
						sendingtodisco = true;
						break;
					}
				}
				if (!sendingtodisco) {
					if (*ii != _lobbysocks[whosturn]) {
						write(*ii, "error", 5);
					}
				}
			}
			cout << "Informed all clients about an error" << endl;
			return;
		}

		//If the message is "win" then send all clients a lost message
		char tmpwin[] = "win";
		if (!strcmp(buffer, tmpwin)) {
			cout << "Received a win" << endl;
			//One client has won the game
			//Send to everyone else that they have lost
			for (ii = _lobbysocks.begin(); ii != _lobbysocks.end(); ii++) {
				bool sendingtodisco = false;
				for (discoii = disconnectv.begin(); discoii != disconnectv.end(); discoii++) {
					if (*ii == *discoii) {
						sendingtodisco = true;
						break;
					}
				}
				if (!sendingtodisco) {
					if (*ii != _lobbysocks[whosturn]) {
						write(*ii, "lost", 4);
					}
				}
			}
			cout << "Informed all clients that they lost" << endl;
			return;
		}


		//Now Broadcast to all clients
		puts("Sending move to everyone");
		for (ii = _lobbysocks.begin(); ii != _lobbysocks.end(); ii++) {
			bool sendingtodisco = false;
			for (discoii = disconnectv.begin(); discoii != disconnectv.end(); discoii++) {
				if (*ii == *discoii) {
					sendingtodisco = true;
					break;
				}
			}
			if (!sendingtodisco) {
				n = write(*ii, buffer, 18);
				if (n < 0) {
					cout << "Error while sending" << endl;
					disconnectv.push_back(*ii);
				}
			}
		}
		cout << endl;
		puts("All clients were sent the move");

		whosturn = buffer[3] - 49; //Set the new player

	}

	for (ii = _lobbysocks.begin(); ii != _lobbysocks.end(); ii++) {
		close(*ii);
	}
}

void acceptAConnection(int &_newsock, bool &_connected, int &_sockfd,
struct sockaddr_in &_cli_addr, socklen_t &_clilen) {
	_newsock = accept(_sockfd, (struct sockaddr *) &_cli_addr, &_clilen);

	_connected = true;
}
