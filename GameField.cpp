/*
 * GameField.cpp
 *
 *  Created on: 25.11.2014
 *      Author: Leon
 */

#include "GameField.h"

GameField::GameField(SDL_Renderer *_renderer, int _rows, int _columns) {
	maxrow = _rows;
	maxcolumn = _columns;

	playercount = 0;
	playeractive = 0;

	yourturn = false;

	renderer = _renderer;

	oneimg = IMG_Load("Resources/Block/OneImg.png");
	twoimg = IMG_Load("Resources/Block/TwoImg.png");
	threeimg = IMG_Load("Resources/Block/ThreeImg.png");
	fourimg = IMG_Load("Resources/Block/FourImg.png");
	fiveimg = IMG_Load("Resources/Block/FiveImg.png");
	redimg = IMG_Load("Resources/Block/RedImg.png");
	blueimg = IMG_Load("Resources/Block/BlueImg.png");
	greenimg = IMG_Load("Resources/Block/GreenImg.png");
	yellowimg = IMG_Load("Resources/Block/YellowImg.png");
	blockbackground = IMG_Load("Resources/Block/BlockBackground.png");
	outline = IMG_Load("Resources/Block/Outline.png");
	hover = IMG_Load("Resources/Block/BlockHover.png");
	turntext = IMG_Load("Resources/UI/Indicator/Turn.png");
	yourturntext = IMG_Load("Resources/UI/Indicator/YourTurn.png");
	outimg = IMG_Load("Resources/UI/Indicator/Out.png");

	outlinetext = SDL_CreateTextureFromSurface(renderer, outline);
	turntexttext = SDL_CreateTextureFromSurface(renderer, turntext);
	turndisplayred = SDL_CreateTextureFromSurface(renderer, redimg);
	turndisplayblue = SDL_CreateTextureFromSurface(renderer, blueimg);
	turndisplaygreen = SDL_CreateTextureFromSurface(renderer, greenimg);
	turndisplayyellow = SDL_CreateTextureFromSurface(renderer, yellowimg);
	yourturndisplay = SDL_CreateTextureFromSurface(renderer, yourturntext);
	turnsmallred = SDL_CreateTextureFromSurface(renderer, redimg);
	turnsmallblue = SDL_CreateTextureFromSurface(renderer, blueimg);
	turnsmallgreen = SDL_CreateTextureFromSurface(renderer, greenimg);
	turnsmallyellow = SDL_CreateTextureFromSurface(renderer, yellowimg);
	outlinesmalltext = SDL_CreateTextureFromSurface(renderer, outline);
	outtext = SDL_CreateTextureFromSurface(renderer, outimg);
	//SDL_FreeSurface(outline);
	//SDL_FreeSurface(turntext);
	//SDL_FreeSurface(outimg);

	outlinepos.w = 80;
	outlinepos.h = 80;
	outlinepos.x = -500;
	outlinepos.y = -500;

	turntextpos.w = 80;
	turntextpos.h = 40;
	turntextpos.x = 520;
	turntextpos.y = 20;

	turndisplaypos.w = 80;
	turndisplaypos.h = 80;
	turndisplaypos.x = 520;
	turndisplaypos.y = 70;

	yourturndisplaypos.w = 80;
	yourturndisplaypos.h = 80;
	yourturndisplaypos.x = 520;
	yourturndisplaypos.y = 170;

	turnsmallredpos.w = 20;
	turnsmallredpos.h = 20;
	turnsmallredpos.x = 520;
	turnsmallredpos.y = 260;

	turnsmallbluepos.w = 20;
	turnsmallbluepos.h = 20;
	turnsmallbluepos.x = 540;
	turnsmallbluepos.y = 260;

	turnsmallgreenpos.w = 20;
	turnsmallgreenpos.h = 20;
	turnsmallgreenpos.x = 560;
	turnsmallgreenpos.y = 260;

	turnsmallyellowpos.w = 20;
	turnsmallyellowpos.h = 20;
	turnsmallyellowpos.x = 580;
	turnsmallyellowpos.y = 260;

	outlinesmall.w = 21;
	outlinesmall.h = 21;
	outlinesmall.x = -500;
	outlinesmall.y = -500;

	int startX = 50;
	int startY = 20;

	for (int i = 0; i < _rows; i++) {
		for (int j = 0; j < _columns; j++) {
			block tmp;
			tmp.background = SDL_CreateTextureFromSurface(renderer,
				blockbackground);
			tmp.onetexture = SDL_CreateTextureFromSurface(renderer, oneimg);
			tmp.twotexture = SDL_CreateTextureFromSurface(renderer, twoimg);
			tmp.threetexture = SDL_CreateTextureFromSurface(renderer, threeimg);
			tmp.fourtexture = SDL_CreateTextureFromSurface(renderer, fourimg);
			tmp.fivetexture = SDL_CreateTextureFromSurface(renderer, fiveimg);
			tmp.row = i;
			tmp.column = j;
			tmp.position.h = 80;
			tmp.position.w = 80;
			tmp.position.x = startX + (j * 90);
			tmp.position.y = startY + (i * 90);

			tmp.playercount = 0;
			tmp.life = 5;
			tmp.lifetodrop = 0;
			tmp.playeronfield[0] = false;
			tmp.playeronfield[1] = false;
			tmp.playeronfield[2] = false;
			tmp.playeronfield[3] = false;

			tmp.hover = SDL_CreateTextureFromSurface(renderer, hover);
			tmp.hoverstate = false;

			tmp.playerred = SDL_CreateTextureFromSurface(renderer, redimg);
			tmp.playerblue = SDL_CreateTextureFromSurface(renderer, blueimg);
			tmp.playergreen = SDL_CreateTextureFromSurface(renderer, greenimg);
			tmp.playeryellow = SDL_CreateTextureFromSurface(renderer,
				yellowimg);

			tmp.smallposred.h = 40;
			tmp.smallposred.w = 40;
			tmp.smallposred.x = tmp.position.x;
			tmp.smallposred.y = tmp.position.y;

			tmp.smallposblue.h = 40;
			tmp.smallposblue.w = 40;
			tmp.smallposblue.x = tmp.position.x + 40;
			tmp.smallposblue.y = tmp.position.y;

			tmp.smallposgreen.h = 40;
			tmp.smallposgreen.w = 40;
			tmp.smallposgreen.x = tmp.position.x;
			tmp.smallposgreen.y = tmp.position.y + 40;

			tmp.smallposyellow.h = 40;
			tmp.smallposyellow.w = 40;
			tmp.smallposyellow.x = tmp.position.x + 40;
			tmp.smallposyellow.y = tmp.position.y + 40;

			tmp.smalltextred = SDL_CreateTextureFromSurface(renderer,
				redimg);
			tmp.smalltextblue = SDL_CreateTextureFromSurface(renderer,
				blueimg);
			tmp.smalltextgreen = SDL_CreateTextureFromSurface(renderer,
				greenimg);
			tmp.smalltextyellow = SDL_CreateTextureFromSurface(renderer,
				yellowimg);

			BlockList.push_back(tmp);
		}
	}

	//SDL_FreeSurface(blockbackground);
	//SDL_FreeSurface(hover);

	PlayerOut.clear();
}

void GameField::ResetField() {
	playercount = 0;

	yourturn = false;

	int startX = 50;
	int startY = 20;

	if (!BlockList.empty()) {
		for (auto&& d : BlockList) {
			d.playercount = 0;
			d.life = 5;
			d.lifetodrop = 0;
			d.playeronfield[0] = false;
			d.playeronfield[1] = false;
			d.playeronfield[2] = false;
			d.playeronfield[3] = false;

			d.hoverstate = false;
		}
	}

	PlayerOut.clear();
}

void GameField::SetPlayerOnField(int _playerid, int _row, int _column) {
	if (!BlockList.empty()) {
		for (auto&& d : BlockList) {
			if (d.row == _row && d.column == _column) {
				d.playercount++;
				d.lifetodrop++;
				d.playeronfield[_playerid] = true;
				return;
			}
		}
	}
}

void GameField::ChangePlayerPosition(int _playerid, int _row, int _column,
	list<int> &_playerout) {
	if (!BlockList.empty()) {
		for (auto&& d : BlockList) {
			//Find Player
			if (d.playeronfield[_playerid]) {
				//Deactivate Player on this field
				d.playeronfield[_playerid] = false;
				d.playercount--;
				//Decrease life of Block
				d.life -= d.lifetodrop;
				d.lifetodrop = d.playercount;
				if (d.life <= 0) { //Block is dead
					d.life = 0;
					if (d.playercount > 0) { //If there are still players on this block, the loose the game
						for (int i = 0; i < 4; i++) {
							if (d.playeronfield[i]) { //Go through all player and check if they are on this Block
								_playerout.push_back(i);
							}
						}
					}
				}
				break;
			}
		}
		//Now set player on his new field
		for (auto&& d : BlockList) {
			//Find new place
			if (d.row == _row && d.column == _column) {
				d.playeronfield[_playerid] = true;
				d.playercount++;
				d.lifetodrop++;
				break;
			}
		}
	}
}

void GameField::MarkActivePlayer(int _playerid) {
	//Display the Player to act
	switch (_playerid) {
	case 0:
		playeractive = 0;
		//Outline the small indicator
		outlinesmall.x = turnsmallredpos.x;
		outlinesmall.y = turnsmallredpos.y;
		break;
	case 1:
		playeractive = 1;
		//Outline the small indicator
		outlinesmall.x = turnsmallbluepos.x;
		outlinesmall.y = turnsmallbluepos.y;
		break;
	case 2:
		playeractive = 2;
		//Outline the small indicator
		outlinesmall.x = turnsmallgreenpos.x;
		outlinesmall.y = turnsmallgreenpos.y;
		break;
	case 3:
		playeractive = 3;
		//Outline the small indicator
		outlinesmall.x = turnsmallyellowpos.x;
		outlinesmall.y = turnsmallyellowpos.y;
		break;
	}

	//Outline of the Block, the player is on
	if (!BlockList.empty()) {
		for (auto&& d : BlockList) {
			if (d.playeronfield[_playerid]) {
				outlinepos.x = d.position.x;
				outlinepos.y = d.position.y;
				return;
			}
		}
	}
}

void GameField::MarkOutPlayer(list<int> &_playerOut) {
	//PlayerOut.clear();
	if (!_playerOut.empty()) {
		list<int>::iterator tmpit;
		for (tmpit = _playerOut.begin(); tmpit != _playerOut.end(); tmpit++) {
			cout << "A player lost" << endl;
			PlayerOut.push_back(*tmpit + 1);
		}
	}
}

bool GameField::CheckIfMoveIsPossible(int _playerid) {
	int tmprow = 0, tmpcolumn = 0;

	//First of find the Block the player is on
	if (!BlockList.empty()) {
		for (auto&& d : BlockList) {
			if (d.playeronfield[_playerid]) {
				tmprow = d.row;
				tmpcolumn = d.column;
				break;
			}
		}
	}

	int countall = 0; //Counts all adjacent blocks
	int countoff = 0; //Count all deactivated adjacent blocks

	//Now search for all adjacent blocks and check there life
	if (!BlockList.empty()) {
		for (auto&& d : BlockList) {
			//This Block is either under or over the player
			if ((d.row >= 0 && d.row <= maxrow)
				&& (d.row == tmprow - 1 || d.row == tmprow + 1)
				&& d.column == tmpcolumn) {
				if (d.life == 0) {
					countall++;
					countoff++;
				} else {
					return true; //Move is possible
				}
			}
			//This Block is either left or right of the player
			if ((d.column >= 0 && d.column <= maxcolumn)
				&& (d.column == tmpcolumn - 1 || d.column == tmpcolumn + 1)
				&& d.row == tmprow) {
				if (d.life == 0) {
					countall++;
					countoff++;
				} else {
					return true; //Move is possible
				}
			}
		}
	}

	if (countall == countoff) { //All adjacent blocks are off
		//Find the block the player ist on again and set its life to 0
		if (!BlockList.empty()) {
			for (auto&& d : BlockList) {
				if (d.playeronfield[_playerid]) {
					d.life = 0;
					break;
				}
			}
		}
		return false;
	}

	return true;
}

void GameField::HoverCheck(int MouseX, int MouseY, int _playerid) {
	int tmprow = 0, tmpcolumn = 0;

	//First of find the Block the player is on
	if (!BlockList.empty()) {
		for (auto&& d : BlockList) {
			if (d.playeronfield[_playerid]) {
				tmprow = d.row;
				tmpcolumn = d.column;
				break;
			}
		}

		for (auto&& d : BlockList) {
			if (MouseX >= d.position.x
				&& MouseX <= (d.position.x + d.position.w)) { //Mouse is on same X level with button
				if (MouseY >= d.position.y
					&& MouseY <= (d.position.y + d.position.h)) { //Mouse is also on same Y level
					//Finds every collision with any Block
					//Now needs to test if this Block is next to the block, the player is on
					//This Block is either under or over the player
					if ((d.row >= 0 && d.row <= maxrow)
						&& (d.row == tmprow - 1 || d.row == tmprow + 1)
						&& d.column == tmpcolumn && d.life >= 1) {
						d.hoverstate = true;
					}
					//This Block is either left or right of the player
					if ((d.column >= 0 && d.column <= maxcolumn)
						&& (d.column == tmpcolumn - 1
						|| d.column == tmpcolumn + 1)
						&& d.row == tmprow && d.life >= 1) {
						d.hoverstate = true;
					}
				} else {
					d.hoverstate = false;
				}
			} else {
				d.hoverstate = false;
			}
		}
	}
}

bool GameField::Click(int MouseX, int MouseY, int _playerid, int &_newrow,
	int &_newcolumn) {
	int tmprow = 0, tmpcolumn = 0;

	//First of find the Block the player is on
	if (!BlockList.empty()) {
		for (auto&& d : BlockList) {
			if (d.playeronfield[_playerid]) {
				tmprow = d.row;
				tmpcolumn = d.column;
				break;
			}
		}

		for (auto&& d : BlockList) {
			if (MouseX >= d.position.x
				&& MouseX <= (d.position.x + d.position.w)) { //Mouse is on same X level with button
				if (MouseY >= d.position.y
					&& MouseY <= (d.position.y + d.position.h)) { //Mouse is also on same Y level
					//Finds every collision with any Block
					//Now needs to test if this Block is next to the block, the player is on
					//This Block is either under or over the player
					if ((d.row >= 0 && d.row <= maxrow)
						&& (d.row == tmprow - 1 || d.row == tmprow + 1)
						&& d.column == tmpcolumn && d.life >= 1) {
						d.hoverstate = false;
						if (d.row == tmprow - 1) {
							_newrow = tmprow - 1;
						} else if (d.row == tmprow + 1) {
							_newrow = tmprow + 1;
						}
						_newcolumn = tmpcolumn;
						return true;
					}
					//This Block is either left or right of the player
					if ((d.column >= 0 && d.column <= maxcolumn)
						&& (d.column == tmpcolumn - 1
						|| d.column == tmpcolumn + 1)
						&& d.row == tmprow && d.life >= 1) {
						d.hoverstate = false;
						if (d.column == tmpcolumn - 1) {
							_newcolumn = tmpcolumn - 1;
						} else if (d.column == tmpcolumn + 1) {
							_newcolumn = tmpcolumn + 1;
						}
						_newrow = tmprow;
						return true;
					}
				}
			}
		}
	}

	return false;
}

void GameField::SetYourTurn(bool _yours) {
	if (_yours) {
		yourturn = true;
	} else {
		yourturn = false;
	}
}

void GameField::SetPlayerCount(int _playercount) {
	playercount = _playercount;
}

void GameField::Render() {
	if (!BlockList.empty()) {
		for (auto&& d : BlockList) {
			if (d.life > 0) {
				//Background
				SDL_RenderCopy(renderer, d.background, NULL, &d.position);

				//Life
				switch (d.life) {
				case 1:
					SDL_RenderCopy(renderer, d.onetexture, NULL, &d.position);
					break;
				case 2:
					SDL_RenderCopy(renderer, d.twotexture, NULL, &d.position);
					break;
				case 3:
					SDL_RenderCopy(renderer, d.threetexture, NULL, &d.position);
					break;
				case 4:
					SDL_RenderCopy(renderer, d.fourtexture, NULL, &d.position);
					break;
				case 5:
					SDL_RenderCopy(renderer, d.fivetexture, NULL, &d.position);
					break;
				}

				//Player
				if (d.playercount == 1) {
					if (d.playeronfield[0]) {
						SDL_RenderCopy(renderer, d.playerred, NULL,
							&d.position);
					} else if (d.playeronfield[1]) {
						SDL_RenderCopy(renderer, d.playerblue, NULL,
							&d.position);
					} else if (d.playeronfield[2]) {
						SDL_RenderCopy(renderer, d.playergreen, NULL,
							&d.position);
					} else if (d.playeronfield[3]) {
						SDL_RenderCopy(renderer, d.playeryellow, NULL,
							&d.position);
					}
				} else if (d.playercount > 1) {
					if (d.playeronfield[0]) {
						SDL_RenderCopy(renderer, d.smalltextred, NULL,
							&d.smallposred);
					}
					if (d.playeronfield[1]) {
						SDL_RenderCopy(renderer, d.smalltextblue, NULL,
							&d.smallposblue);
					}
					if (d.playeronfield[2]) {
						SDL_RenderCopy(renderer, d.smalltextgreen, NULL,
							&d.smallposgreen);
					}
					if (d.playeronfield[3]) {
						SDL_RenderCopy(renderer, d.smalltextyellow, NULL,
							&d.smallposyellow);
					}
				}

				//Hover
				if (d.hoverstate) {
					SDL_RenderCopy(renderer, d.hover, NULL, &d.position);
				}
			}
		}
	}

	//Turn Text
	SDL_RenderCopy(renderer, turntexttext, NULL, &turntextpos);

	//Turn Display
	switch (playeractive) {
	case 0:
		SDL_RenderCopy(renderer, turndisplayred, NULL, &turndisplaypos);
		break;
	case 1:
		SDL_RenderCopy(renderer, turndisplayblue, NULL, &turndisplaypos);
		break;
	case 2:
		SDL_RenderCopy(renderer, turndisplaygreen, NULL, &turndisplaypos);
		break;
	case 3:
		SDL_RenderCopy(renderer, turndisplayyellow, NULL, &turndisplaypos);
		break;
	}

	//Your Turn Display
	if (yourturn) {
		SDL_RenderCopy(renderer, yourturndisplay, NULL, &yourturndisplaypos);
	}

	//Small list of all players in the game
	if (playercount == 1) {
		SDL_RenderCopy(renderer, turnsmallred, NULL, &turnsmallredpos);
	} else if (playercount == 2) {
		SDL_RenderCopy(renderer, turnsmallred, NULL, &turnsmallredpos);
		SDL_RenderCopy(renderer, turnsmallblue, NULL, &turnsmallbluepos);
	} else if (playercount == 3) {
		SDL_RenderCopy(renderer, turnsmallred, NULL, &turnsmallredpos);
		SDL_RenderCopy(renderer, turnsmallblue, NULL, &turnsmallbluepos);
		SDL_RenderCopy(renderer, turnsmallgreen, NULL, &turnsmallgreenpos);
	} else if (playercount == 4) {
		SDL_RenderCopy(renderer, turnsmallred, NULL, &turnsmallredpos);
		SDL_RenderCopy(renderer, turnsmallblue, NULL, &turnsmallbluepos);
		SDL_RenderCopy(renderer, turnsmallgreen, NULL, &turnsmallgreenpos);
		SDL_RenderCopy(renderer, turnsmallyellow, NULL, &turnsmallyellowpos);
	}

	//Active Player
	SDL_RenderCopy(renderer, outlinetext, NULL, &outlinepos);
	SDL_RenderCopy(renderer, outlinesmalltext, NULL, &outlinesmall);

	//Mark players that are out
	if (!PlayerOut.empty()) {
		list<int>::iterator tmpit;
		for (tmpit = PlayerOut.begin(); tmpit != PlayerOut.end(); tmpit++) {
			if (*tmpit == 1) {
				SDL_RenderCopy(renderer, outtext, NULL, &turnsmallredpos);
			}
			if (*tmpit == 2) {
				SDL_RenderCopy(renderer, outtext, NULL, &turnsmallbluepos);
			}
			if (*tmpit == 3) {
				SDL_RenderCopy(renderer, outtext, NULL, &turnsmallgreenpos);
			}
			if (*tmpit == 4) {
				SDL_RenderCopy(renderer, outtext, NULL, &turnsmallyellowpos);
			}
		}
	}
}

GameField::~GameField() {
	// TODO Auto-generated destructor stub
}