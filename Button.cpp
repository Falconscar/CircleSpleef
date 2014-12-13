/*
 * Button.cpp
 *
 *  Created on: 18.06.2014
 *      Author: Leon
 */

#include "Button.h"

Button::Button() {}

Button::Button(const char *file, const char *blendfile, SDL_Renderer * renderer, int x, int y, int w,
	int h) {
	this->renderer = renderer;
	surface = IMG_Load(file);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	blendsurface = IMG_Load(blendfile);
	blendtexture = SDL_CreateTextureFromSurface(renderer, blendsurface);

	SDL_FreeSurface(surface);
	SDL_FreeSurface(blendsurface);

	position.x = x;
	position.y = y;
	position.w = w;
	position.h = h;

	isclickable = true;
	mouseover = false;
}

void Button::Change(const char *file, SDL_Renderer * renderer, int x, int y,
	int w, int h) {
	//Not up to date
	this->renderer = renderer;
	surface = IMG_Load(file);
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
	SDL_FreeSurface(blendsurface);

	position.x = x;
	position.y = y;
	position.w = w;
	position.h = h;

	isclickable = true;
	mouseover = false;
}

void Button::ChangeImage(const char *file) {
	surface = IMG_Load(file);
	SDL_DestroyTexture(texture);
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);
}

void Button::FlipStateOfClickable() {
	if (isclickable) {
		isclickable = false;
	} else if (!isclickable) {
		isclickable = true;
	}
}

void Button::RenderCopy(SDL_Renderer * renderer) {
	SDL_RenderCopy(renderer, texture, NULL, &position);
	if (mouseover) {
		SDL_RenderCopy(renderer, blendtexture, NULL, &position);
	}
}

/*
 * Click Method checks for a collision between the Cursor and a Button
 */
bool Button::Click(int MouseX, int MouseY) {
	if (isclickable) {
		if (MouseX >= position.x && MouseX <= (position.x + position.w)) { //Mouse is on same X level with button
			if (MouseY >= position.y && MouseY <= (position.y + position.h)) { //Mouse is also on same Y level
				return true;
			}
		}
	}
	return false;
}

void Button::MouseOver(int MouseX, int MouseY) {
	if (MouseX >= position.x && MouseX <= (position.x + position.w)) { //Mouse is on same X level with button
		if (MouseY >= position.y && MouseY <= (position.y + position.h)) { //Mouse is also on same Y level
			mouseover = true;
		} else {
			mouseover = false;
		}
	} else {
		mouseover = false;
	}
}

Button::~Button() {
	//SDL_DestroyTexture(texture);
	//SDL_FreeSurface(surface);
}