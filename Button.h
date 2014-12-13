/*
 * Button.h
 *
 *  Created on: 18.06.2014
 *      Author: Leon
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
using namespace std;

class Button {
private:
	SDL_Renderer *renderer;
	SDL_Surface *surface, *blendsurface;
	SDL_Texture *texture, *blendtexture;
	SDL_Rect position;

	bool isclickable;
	bool mouseover;
public:
	Button();
	Button(const char *file, const char *blendfile, SDL_Renderer * renderer, int x, int y, int w, int h);
	void Change(const char *file, SDL_Renderer * renderer, int x, int y, int w, int h);
	void ChangeImage(const char *file);
	void FlipStateOfClickable();
	void RenderCopy(SDL_Renderer * renderer);
	bool Click(int MouseX, int MouseY);
	void MouseOver(int MouseX, int MouseY);
	virtual ~Button();
};

#endif /* BUTTON_H_ */
