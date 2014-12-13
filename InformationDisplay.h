#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <list>
#include <time.h>

#include <iostream>
using namespace std;


class InformationDisplay {
private:
	SDL_Renderer *renderer;
	SDL_Surface *infoimg, *errorimg;
	SDL_Texture *infotext, *errortext;
	SDL_Rect inpos[7]; //Positions inside the img
	SDL_Rect relpos[7], errorrelpos; //Positions on the window

	int whattorender, whattorendererror, errordelay, starttime;
	bool erroractive;

public:
	InformationDisplay();
	InformationDisplay(SDL_Renderer *_renderer);
	void SetRenderer(SDL_Renderer *_renderer);
	void SetToRender(int _whattorender);
	void SetError(int _whattorendererror);
	void ClearAllErrors();
	void RenderInfo();
	void RenderError();
	~InformationDisplay();
};

