/*
 * GameObject.cpp
 *
 *  Created on: 25.11.2014
 *      Author: Leon
 */

#include "GameObject.h"

GameObject::GameObject(SDL_Renderer *_renderer, SDL_Window *_window) {
	window = _window;
	renderer = _renderer;
	fieldptr = nullptr;
	state = 0;

	titleimg = IMG_Load("Resources/Misc/Title.png");
	titletext = SDL_CreateTextureFromSurface(renderer, titleimg);
	howtoplayimg = IMG_Load("Resources/Misc/HowToPlay.png");
	howtoplaytext = SDL_CreateTextureFromSurface(renderer, howtoplayimg);
	versionimg = IMG_Load("Resources/Misc/Version.png");
	versiontext = SDL_CreateTextureFromSurface(renderer, versionimg);
	creditsimg = IMG_Load("Resources/Misc/Credits.png");
	creditstext = SDL_CreateTextureFromSurface(renderer, creditsimg);
	SDL_FreeSurface(titleimg);
	SDL_FreeSurface(howtoplayimg);
	SDL_FreeSurface(versionimg);
	SDL_FreeSurface(creditsimg);

	titlepos.x = 110;
	titlepos.y = 30;
	titlepos.w = 420;
	titlepos.h = 155;

	howtoplaypos.x = 16;
	howtoplaypos.y = 0;
	howtoplaypos.w = 500;
	howtoplaypos.h = 480;

	versionpos.x = 0;
	versionpos.y = 467;
	versionpos.w = 50;
	versionpos.h = 10;

	creditspos.x = 508;
	creditspos.y = 463;
	creditspos.w = 129;
	creditspos.h = 14;
}

void GameObject::SetField(GameField *_fieldptr) {
	fieldptr = _fieldptr;
}

int GameObject::GetState() {
	return state;
}

void GameObject::SetState(int _newstate) {
	state = _newstate;

	if (_newstate == 0) {
		fieldptr->ResetField();
		SDL_SetWindowTitle(window, "Circle Spleef");
	}
}

void GameObject::Render() {
	if (state == 0 || state == -1) {
		//Title
		SDL_RenderCopy(renderer, titletext, NULL, &titlepos);
		//Version
		SDL_RenderCopy(renderer, versiontext, NULL, &versionpos);
		//Credits
		SDL_RenderCopy(renderer, creditstext, NULL, &creditspos);
	} else if (state == 2) {
		//How to play
		SDL_RenderCopy(renderer, howtoplaytext, NULL, &howtoplaypos);
	}
}

GameObject::~GameObject() {
	// TODO Auto-generated destructor stub
}