//============================================================================
// Name        : main.cpp
// Author      : Leon Eck
// Version     : 1.0
// Copyright   : Copyright Leon Eck 2014
//============================================================================

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <SDL.h>

#include <winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <thread>
#include <string>
#include <sstream>
using namespace std;

#include "Button.h"
#include "GameObject.h"
//#include "GameField.h"  //Already done by the GameObject
#include "InformationDisplay.h"
#include "Socketstuff.hpp"

/*
 * Variables
 */
//Game
int numberofplayers = 0;
bool *playeralive;
int whosturn = 1;
int newrow = 0, newcolumn = 0; //Logic for input handling

//Window
SDL_Window *window;
SDL_Renderer *windowrenderer;
SDL_Surface *icon;

//GameLogic
bool quit = false;
SDL_Event event;
bool leftMouseButtonDown = false;
int startPositionRow[4] = { 0, 0, 4, 4 };
int startPositionColumn[4] = { 0, 4, 0, 4 };
int countoffplayers = 0; //For counting players that have lost

//Mulitplayer Variables
bool multiplayerwasinitialized = false; //Indicates if the connection to the server was setup
bool multiplayerstarted = false; //The game you tried to join is starting
bool allowedtogetmove = true; //Only allowed once per round
bool receivedstatus = false; //Move was completly received
int tmpmplayer = -1, tmpmrow = -1, tmpmcolumn = -1, tmpmnewplayer = -1;
bool errorreset = false;
int connectionstarttime = 0, connectiontimeout = 5;
bool tryingconnecting = false; //Timeout for server connection

int main(int argc, char *argv[]) {  //int argc, char ** argv
	/*
	 * Startup
	 */

	srand(static_cast<unsigned int> (time(NULL)));

	/*--- SDL-Initialization ---*/
	SDL_Init(SDL_INIT_EVERYTHING);

	icon = IMG_Load("Resources/Misc/Icon.png");

	//Create Window
	SDL_Rect resolution;
	SDL_GetDisplayBounds(0, &resolution);
	window = SDL_CreateWindow("Circle Spleef", (resolution.w / 2) - 320,
		(resolution.h / 2) - 240, 640, 480, SDL_WINDOW_OPENGL); //SDL_WINDOW_BORDERLESS
	SDL_SetWindowIcon(window, icon);
	windowrenderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	GameObject Game(windowrenderer, window);

	Button PlayGameButton("Resources/UI/Buttons/PlayGame.png", "Resources/UI/Buttons/Outline.png", windowrenderer, 170, 215,
		300, 40);
	Button MultiplayerButton("Resources/UI/Buttons/Multiplayer.png", "Resources/UI/Buttons/Outline.png", windowrenderer,
		170, 285, 300, 40);
	Button HotToPlayButton("Resources/UI/Buttons/HowToPlay.png", "Resources/UI/Buttons/Outline.png", windowrenderer, 170, 355, 300, 40);

	//Number Buttons to select the number of players in an offline game
	Button TwoPlayerButton("Resources/UI/NumberButtons/2.png", "Resources/UI/NumberButtons/Outline.png", windowrenderer, 215, 215, 50, 40);
	Button ThreePlayerButton("Resources/UI/NumberButtons/3.png", "Resources/UI/NumberButtons/Outline.png", windowrenderer, 300, 215, 50, 40);
	Button FourPlayerButton("Resources/UI/NumberButtons/4.png", "Resources/UI/NumberButtons/Outline.png", windowrenderer, 385, 215, 50, 40);

	Button BackButton("Resources/UI/Buttons/Back.png", "Resources/UI/Buttons/OutlineSmall.png", windowrenderer, 515, 400, 100, 40);

	GameField Field(windowrenderer, 5, 5);
	Game.SetField(&Field);

	InformationDisplay *InfoDisp;
	InfoDisp = new InformationDisplay;
	InfoDisp->SetRenderer(windowrenderer);

	while (!quit) {
		SDL_PollEvent(&event);

		if (Game.GetState() == 1) {
			//Set Active Player
			Field.MarkActivePlayer(whosturn - 1);
		}

		/*
		 * Logic
		 */

		//MainMenu Mouseover
		if (Game.GetState() == 0) {
			PlayGameButton.MouseOver(event.motion.x, event.motion.y);
			MultiplayerButton.MouseOver(event.motion.x, event.motion.y);
			HotToPlayButton.MouseOver(event.motion.x, event.motion.y);
		}

		//Numberblocks MouseOver
		if (Game.GetState() == -1) {
			//OnePlayerButton.MouseOver(event.motion.x, event.motion.y);
			TwoPlayerButton.MouseOver(event.motion.x, event.motion.y);
			ThreePlayerButton.MouseOver(event.motion.x, event.motion.y);
			FourPlayerButton.MouseOver(event.motion.x, event.motion.y);
			MultiplayerButton.MouseOver(event.motion.x, event.motion.y);
			HotToPlayButton.MouseOver(event.motion.x, event.motion.y);
		}

		//BackButton MouseOver
		if (Game.GetState() == 2 || Game.GetState() == 3 || Game.GetState() == 1) {
			BackButton.MouseOver(event.motion.x, event.motion.y);
		}

		//Player MouseOver Block Offline
		if (Game.GetState() == 1) {
			Field.HoverCheck(event.motion.x, event.motion.y, whosturn - 1);
		}

		//Player MouseOver Block Online
		if (Game.GetState() == 21 && mul_whosturn == clientID) {
			Field.HoverCheck(event.motion.x, event.motion.y, mul_whosturn - 1);
		}

		//Multiplayer specific logic
		if (Game.GetState() == 20) {
			if (!multiplayerwasinitialized) {
				multiplayerwasinitialized = true;
				connectionstarttime = static_cast<int> (time(NULL));
				tryingconnecting = true;
				thread connecth(InitializeSocketConnection, InfoDisp, &Game);
				connecth.detach();
			}
			if (connected) {
				tryingconnecting = false;
				InfoDisp->SetToRender(1);
				connected = false;
				thread jointh(JoinGame, ref(multiplayerstarted), ref(errorreset), InfoDisp, &Game);
				jointh.detach();
			} else if (tryingconnecting && time(NULL) - connectionstarttime >= connectiontimeout) {
				cout << "Back to menu" << endl;
				InfoDisp->SetError(0);
				Game.SetState(0);
			}
			if (errorreset) {
				cout << "Back to menu" << endl;
				Game.SetState(0);
			}
			if (multiplayerstarted) {
				multiplayerstarted = false; //Reset the value
				clientID++;
				cout << "You are player number: " << clientID << endl;
				switch (clientID) {
				case 1:
					SDL_SetWindowTitle(window, "Circle Spleef - Red");
					break;
				case 2:
					SDL_SetWindowTitle(window, "Circle Spleef - Blue");
					break;
				case 3:
					SDL_SetWindowTitle(window, "Circle Spleef - Green");
					break;
				case 4:
					SDL_SetWindowTitle(window, "Circle Spleef - Yellow");
					break;
				}
				puts("Loading Gamefield");
				//Setting up multiplayer values
				delete[] mul_playeralive;
				mul_playeralive = new bool[mul_numberofplayers];
				//Spawn in Player
				for (int i = 1; i <= mul_numberofplayers; i++) {
					Field.SetPlayerOnField(i - 1, startPositionRow[i - 1],
						startPositionColumn[i - 1]);
					mul_playeralive[i - 1] = true;
				}

				/*
				 After everything is setup for a multiplayer game, the game state is set to 21
				 */
				Game.SetState(21);
				Field.SetPlayerCount(mul_numberofplayers);
				Field.MarkActivePlayer(mul_whosturn - 1);
				if (mul_whosturn == clientID) {
					Field.SetYourTurn(true);
				} else {
					Field.SetYourTurn(false);
				}
			}
		}

		//It is not your turn in multiplayer
		if (Game.GetState() == 21 && mul_whosturn != clientID) {
			Field.SetPlayerCount(mul_numberofplayers);
			Field.MarkActivePlayer(mul_whosturn - 1);

			Field.SetYourTurn(false);

			if (allowedtogetmove) {
				allowedtogetmove = false;
				thread getmoveth(GetMove, ref(tmpmplayer), ref(tmpmrow), ref(tmpmcolumn), ref(tmpmnewplayer), ref(receivedstatus), InfoDisp, &Game);
				getmoveth.detach();
			}
			if (receivedstatus) {
				cout << "Received a valid message from client: " << tmpmplayer << endl;
				receivedstatus = false;
				//Check that this message is not from you
				if (clientID != tmpmplayer) {
					mul_whosturn = tmpmnewplayer;
					//cout << mul_whosturn << " | " << clientID << endl;
				}
				Field.MarkActivePlayer(mul_whosturn - 1);
				if (mul_whosturn != clientID) {
					allowedtogetmove = true;
				}
				/*
					Interpretate the opponents move ----
					*/
				if (clientID != tmpmplayer) {
					list<int> playerloose; //A list to store the players in, that loose the game in this round
					list<int>::iterator playerlooseii;

					try { //I have no idea why this needs to be here, but without it the program crashes
						playerloose.push_back(1);
						playerloose.clear();
					}
					catch (const exception &e) {
						cout << e.what() << endl;
					}

					Field.ChangePlayerPosition(tmpmplayer - 1, tmpmrow,
						tmpmcolumn, playerloose);

					if (!playerloose.empty()) {
						for (playerlooseii = playerloose.begin(); playerlooseii != playerloose.end();
							playerlooseii++) { //Go through the list of players that lost the game
							mul_playeralive[*playerlooseii] = false;
						}

						//Mark the players that are out of the game
						Field.MarkOutPlayer(playerloose);
					}

					//Check for all players if they can still move
					playerloose.clear();
					for (int i = 0; i < mul_numberofplayers; i++) {
						if (mul_playeralive[i]) {
							if (!Field.CheckIfMoveIsPossible(i)) {
								mul_playeralive[i] = false;
								playerloose.push_back(i);
							}
						}

						//Mark the players that are out of the game
						Field.MarkOutPlayer(playerloose);
					}

					//Send players that are out to the Field
					for (int i = 0; i < mul_numberofplayers; i++) {
						if (mul_playeralive[i]) {
							list<int> tmpTmpList;
							tmpTmpList.push_back(i + 1);
						}
					}

					//Check if all players lost
					countoffplayers = 0;
					for (int i = 0; i < mul_numberofplayers; i++) {
						if (!mul_playeralive[i]) {
							countoffplayers++;
						}
					}
					if (countoffplayers == mul_numberofplayers) {
						InfoDisp->SetError(4);
						Game.SetState(0);
					}
				}
				/*
					-----------------------
					*/

				if (mul_whosturn == clientID) {
					Field.SetYourTurn(true);
				} else {
					Field.SetYourTurn(false);
				}
			}
		}

		//Check if you are out of the multiplayer game
		if (Game.GetState() == 21 && !mul_playeralive[clientID - 1]) {
			cout << "You are out of the game" << endl;
			InfoDisp->SetError(4);
			Game.SetState(0);
		}

		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			if (Game.GetState() == 20 || Game.GetState() == 21) {
				message = "disconnect";
				if (send(s, message, strlen(message), 0) < 0) {
					puts("Send failed");
				}
			}
			//closesocket(s);
			//WSACleanup();
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				leftMouseButtonDown = true;
				InfoDisp->ClearAllErrors();
			}
			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (leftMouseButtonDown) { //Click is only a real click if Mousebutton was down and now up again
					leftMouseButtonDown = false;
					if (Game.GetState() == -1) { //Number of players selection
						if (TwoPlayerButton.Click(event.motion.x,
							event.motion.y)) {
							numberofplayers = 2;
							playeralive = new bool[numberofplayers];
							//Spawn in Player
							for (int i = 1; i <= numberofplayers; i++) {
								Field.SetPlayerOnField(i - 1, startPositionRow[i - 1],
									startPositionColumn[i - 1]);
								playeralive[i - 1] = true;
							}
							Field.SetPlayerCount(numberofplayers);
							whosturn = rand() % numberofplayers + 1;
							Game.SetState(1);
						} else if (ThreePlayerButton.Click(event.motion.x,
							event.motion.y)) {
							numberofplayers = 3;
							playeralive = new bool[numberofplayers];
							//Spawn in Player
							for (int i = 1; i <= numberofplayers; i++) {
								Field.SetPlayerOnField(i - 1, startPositionRow[i - 1],
									startPositionColumn[i - 1]);
								playeralive[i - 1] = true;
							}
							Field.SetPlayerCount(numberofplayers);
							whosturn = rand() % numberofplayers + 1;
							Game.SetState(1);
						} else if (FourPlayerButton.Click(event.motion.x,
							event.motion.y)) {
							numberofplayers = 4;
							playeralive = new bool[numberofplayers];
							//Spawn in Player
							for (int i = 1; i <= numberofplayers; i++) {
								Field.SetPlayerOnField(i - 1, startPositionRow[i - 1],
									startPositionColumn[i - 1]);
								playeralive[i - 1] = true;
							}
							Field.SetPlayerCount(numberofplayers);
							whosturn = rand() % numberofplayers + 1;
							Game.SetState(1);
						}
					}
					if (Game.GetState() == 0) { //Main Menu
						if (PlayGameButton.Click(event.motion.x,
							event.motion.y)) {
							Game.SetState(-1);
						}
					}
					if (Game.GetState() == 0 || Game.GetState() == -1) {
						if (MultiplayerButton.Click(event.motion.x,
							event.motion.y)) {
							errorreset = false;
							multiplayerwasinitialized = false;
							multiplayerstarted = false;
							allowedtogetmove = true;
							receivedstatus = false;
							connected = false;
							clientID = -1;
							int tmpmplayer = -1, tmpmrow = -1, tmpmcolumn = -1, tmpmnewplayer = -1;
							bool errorreset = false;
							InfoDisp->SetToRender(0);
							multiplayerwasinitialized = false;
							Game.SetState(20);
						} else if (HotToPlayButton.Click(event.motion.x,
							event.motion.y)) {
							Game.SetState(2);
						}
					} else if (Game.GetState() == 1) { //Offline Play
						newrow = 0;
						newcolumn = 0;
						if (Field.Click(event.motion.x, event.motion.y,
							whosturn - 1, newrow, newcolumn)) {
							list<int> playerloose; //A list to store the players in, that loose the game in this round
							list<int>::iterator playerlooseii;

							try { //I have no idea why this needs to be here, but without it the program crashes
								playerloose.push_back(1);
								playerloose.clear();
							}
							catch (const exception &e) {
								cout << e.what() << endl;
							}

							Field.ChangePlayerPosition(whosturn - 1, newrow,
								newcolumn, playerloose);

							if (!playerloose.empty()) {
								for (playerlooseii = playerloose.begin(); playerlooseii != playerloose.end();
									playerlooseii++) { //Go through the list of players that lost the game
									playeralive[*playerlooseii] = false;
								}

								//Mark the players that are out of the game
								Field.MarkOutPlayer(playerloose);
							}

							//Check for all players if they can still move
							for (int i = 0; i < numberofplayers; i++) {
								if (playeralive[i]) {
									if (!Field.CheckIfMoveIsPossible(i)) {
										playeralive[i] = false;
									}
								}

								//Mark the players that are out of the game
								Field.MarkOutPlayer(playerloose);
							}

							//Check if all players lost
							countoffplayers = 0;
							for (int i = 0; i < numberofplayers; i++) {
								if (!playeralive[i]) {
									countoffplayers++;
								}
							}
							if (countoffplayers == numberofplayers) {
								InfoDisp->SetError(4);
								Game.SetState(0);
							}

							if (whosturn == numberofplayers) {
								whosturn = 1;
								while (!playeralive[whosturn - 1]) {
									if (whosturn == numberofplayers) {
										whosturn = 1;
									} else {
										whosturn++;
									}
								}
							} else {
								whosturn++;
								while (!playeralive[whosturn - 1]) {
									if (whosturn == numberofplayers) {
										whosturn = 1;
									} else {
										whosturn++;
									}
								}
							}
							Field.MarkActivePlayer(whosturn - 1);
						}
					} else if (Game.GetState() == 21 && mul_whosturn == clientID) { //This is the part, where the client is active and will perform the action
						newrow = 0;
						newcolumn = 0;
						if (Field.Click(event.motion.x, event.motion.y,
							mul_whosturn - 1, newrow, newcolumn)) {
							list<int> playerloose; //A list to store the players in, that loose the game in this round
							list<int>::iterator playerlooseii;

							try { //I have no idea why this needs to be here, but without it the program crashes
								playerloose.push_back(1);
								playerloose.clear();
							}
							catch (const exception &e) {
								cout << e.what() << endl;
							}

							Field.ChangePlayerPosition(mul_whosturn - 1, newrow,
								newcolumn, playerloose);

							if (!playerloose.empty()) {
								for (playerlooseii = playerloose.begin(); playerlooseii != playerloose.end();
									playerlooseii++) { //Go through the list of players that lost the game
									mul_playeralive[*playerlooseii] = false;
								}

								//Mark the players that are out of the game
								Field.MarkOutPlayer(playerloose);
							}

							//Check for all players if they can still move
							playerloose.clear();
							for (int i = 0; i < mul_numberofplayers; i++) {
								if (mul_playeralive[i]) {
									if (!Field.CheckIfMoveIsPossible(i)) {
										mul_playeralive[i] = false;
										playerloose.push_back(i);
									}
								}

								//Mark the players that are out of the game
								Field.MarkOutPlayer(playerloose);
							}

							//Check if all players lost
							countoffplayers = 0;
							for (int i = 0; i < mul_numberofplayers; i++) {
								if (!mul_playeralive[i]) {
									countoffplayers++;
								}
							}
							if (countoffplayers == mul_numberofplayers) {
								return 0;
							}if (countoffplayers == mul_numberofplayers - 1) {
								//Only one player is alive
								//Check if this is you
								if (mul_playeralive[clientID - 1]) {
									cout << "You won" << endl;
									InfoDisp->SetError(5);
									message = "win";
									if (send(s, message, strlen(message), 0) < 0) {
										puts("Send failed");
									}
									Game.SetState(0);
								}
							}

							if (mul_whosturn == mul_numberofplayers) {
								mul_whosturn = 1;
								while (!mul_playeralive[mul_whosturn - 1]) {
									if (mul_whosturn == mul_numberofplayers) {
										mul_whosturn = 1;
									} else {
										mul_whosturn++;
									}
								}
							} else {
								mul_whosturn++;
								while (!mul_playeralive[mul_whosturn - 1]) {
									if (mul_whosturn == mul_numberofplayers) {
										mul_whosturn = 1;
									} else {
										mul_whosturn++;
									}
								}
							}
							Field.MarkActivePlayer(mul_whosturn - 1);
							//Now also send this move to the server
							SendMove(newrow, newcolumn, mul_whosturn - 1);
							allowedtogetmove = true;
						}
					}
					if (Game.GetState() == 2 || Game.GetState() == 3 || Game.GetState() == 1) {
						if (BackButton.Click(event.motion.x, event.motion.y)) {
							Game.SetState(0);
						}
					}
				}
			}
			break;
		}

		/*
		 * Render
		 */

		SDL_RenderClear(windowrenderer);
		Game.Render();
		if (Game.GetState() == 0) {
			PlayGameButton.RenderCopy(windowrenderer);
			MultiplayerButton.RenderCopy(windowrenderer);
			HotToPlayButton.RenderCopy(windowrenderer);
		} else if (Game.GetState() == -1) {
			//OnePlayerButton.RenderCopy(windowrenderer);
			TwoPlayerButton.RenderCopy(windowrenderer);
			ThreePlayerButton.RenderCopy(windowrenderer);
			FourPlayerButton.RenderCopy(windowrenderer);
			MultiplayerButton.RenderCopy(windowrenderer);
			HotToPlayButton.RenderCopy(windowrenderer);
		} else if (Game.GetState() == 1) {
			Field.Render();
			BackButton.RenderCopy(windowrenderer);
		} else if (Game.GetState() == 21) {
			Field.Render();
		} else if (Game.GetState() == 20) {
			InfoDisp->RenderInfo();
		} else if (Game.GetState() == 2 || Game.GetState() == 3) {
			BackButton.RenderCopy(windowrenderer);
		}
		InfoDisp->RenderError();

		SDL_RenderPresent(windowrenderer);
	}

	return 0;
}