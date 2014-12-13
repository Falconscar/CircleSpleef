/*
 * GameField.h
 *
 *  Created on: 25.11.2014
 *      Author: Leon
 */

#ifndef GAMEFIELD_H_
#define GAMEFIELD_H_

#include <SDL.h>
#include <SDL_image.h>
#include <list>

#include <iostream>
using namespace std;

class GameField {
private:
	int maxrow, maxcolumn;
	int playercount, playeractive;

	bool yourturn;

	SDL_Renderer *renderer;
	SDL_Surface *oneimg, *twoimg, *threeimg, *fourimg, *fiveimg,
		*redimg, *blueimg, *greenimg, *yellowimg, *blockbackground,
		*outline, *hover, *turntext, *yourturntext, *outimg;
	SDL_Texture *outlinetext, *turntexttext, *turndisplayred, *turndisplayblue, *turndisplaygreen, *turndisplayyellow, *yourturndisplay, *turnsmallred, *turnsmallblue, *turnsmallgreen, *turnsmallyellow, *outlinesmalltext, *outtext;
	SDL_Rect outlinepos, turntextpos, turndisplaypos, yourturndisplaypos, turnsmallredpos, turnsmallbluepos, turnsmallgreenpos, turnsmallyellowpos, outlinesmall;

	struct block {
		SDL_Texture *onetexture, *twotexture, *threetexture, *fourtexture, *fivetexture, *background, *playerred, *playerblue, *playergreen, *playeryellow, *hover; //Which image should be displayed
		SDL_Rect position;
		//If there is more than one player on a field, than these get used
		SDL_Texture *smalltextred, *smalltextblue, *smalltextgreen,
			*smalltextyellow;
		SDL_Rect smallposred, smallposblue, smallposgreen, smallposyellow;
		int row, column; //Position
		int playercount; //Number of players on field
		int life;
		int lifetodrop; //Amount of life that should get erased
		bool playeronfield[4]; // 0 = red, 1 = blue, 2 = green, 3 = yellow
		bool hoverstate; //Set to true if the player hovers over it
	};

	list<block> BlockList;
	list<block>::iterator ii;

	list<int> PlayerOut;
public:
	GameField(SDL_Renderer *_renderer, int _rows, int _columns);
	void ResetField();
	void SetPlayerOnField(int _playerid, int _row, int _column);
	void ChangePlayerPosition(int _playerid, int _row, int _column,
		list<int> &_playerout);
	void MarkActivePlayer(int _playerid);
	void MarkOutPlayer(list<int> &_playerOut);
	bool CheckIfMoveIsPossible(int _playerid);
	void HoverCheck(int MouseX, int MouseY, int _playerid);
	bool Click(int MouseX, int MouseY, int _playerid, int &_newrow,
		int &_newcolumn);
	void SetYourTurn(bool _yours);
	void SetPlayerCount(int _playercount);
	void Render();
	virtual ~GameField();
};

#endif /* GAMEFIELD_H_ */
