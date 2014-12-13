/*
 * GameObject.h
 *
 *  Created on: 25.11.2014
 *      Author: Leon
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include "GameField.h"

class GameObject {
private:
	int state;

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *titleimg, *howtoplayimg, *versionimg, *creditsimg;
	SDL_Texture *titletext, *howtoplaytext, *versiontext, *creditstext;
	SDL_Rect titlepos, howtoplaypos, versionpos, creditspos;

	GameField *fieldptr;
	/*
	 * 0 = Main Menu
	 * 1 = Playing Offline
	 * 2 = How to play
	 */
public:
	GameObject(SDL_Renderer *_renderer, SDL_Window *_window);
	void SetField(GameField *_fieldptr);
	int GetState();
	void SetState(int _newstate);
	void Render();
	virtual ~GameObject();
};

#endif /* GAMEOBJECT_H_ */
