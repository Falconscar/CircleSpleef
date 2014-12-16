#include "InformationDisplay.h"

InformationDisplay::InformationDisplay() {
	renderer = nullptr;

	infoimg = IMG_Load("Resources/Text/Info.png");
	errorimg = IMG_Load("Resources/Text/Error.png");

	infotext = nullptr;
	errortext = nullptr;

	for (int i = 0; i < 7; i++) {
		inpos[i].w = 400;
		inpos[i].h = 50;
		inpos[i].x = 0;
		inpos[i].y = i * 50;
	}

	for (int i = 0; i < 7; i++) {
		relpos[i].w = 400;
		relpos[i].h = 50;
		relpos[i].x = 120;
		relpos[i].y = 100;
	}

	errorrelpos.w = 400;
	errorrelpos.h = 50;
	errorrelpos.x = 120;
	errorrelpos.y = 410;

	whattorender = 0;
	whattorendererror = 0;
	errordelay = 10;
	starttime = static_cast<int> (time(NULL));
	erroractive = false;
}

InformationDisplay::InformationDisplay(SDL_Renderer *_renderer) {
	renderer = _renderer;

	infoimg = IMG_Load("Resources/Text/Info.png");
	errorimg = IMG_Load("Resources/Text/Error.png");

	infotext = SDL_CreateTextureFromSurface(renderer, infoimg);
	errortext = SDL_CreateTextureFromSurface(renderer, errorimg);

	SDL_FreeSurface(infoimg);
	SDL_FreeSurface(errorimg);

	for (int i = 0; i < 7; i++) {
		inpos[i].w = 400;
		inpos[i].h = 50;
		inpos[i].x = 0;
		inpos[i].y = i * 50;
	}

	for (int i = 0; i < 7; i++) {
		relpos[i].w = 400;
		relpos[i].h = 50;
		relpos[i].x = 120;
		relpos[i].y = 100;
	}

	errorrelpos.w = 400;
	errorrelpos.h = 50;
	errorrelpos.x = 120;
	errorrelpos.y = 410;

	whattorender = 0;
	whattorendererror = 0;
	errordelay = 5;
	starttime = static_cast<int> (time(NULL));
	erroractive = false;
}

void InformationDisplay::SetRenderer(SDL_Renderer *_renderer) {
	renderer = _renderer;

	infotext = SDL_CreateTextureFromSurface(renderer, infoimg);
	errortext = SDL_CreateTextureFromSurface(renderer, errorimg);

	SDL_FreeSurface(infoimg);
	SDL_FreeSurface(errorimg);
}

void InformationDisplay::SetToRender(int _whattorender) {
	whattorender = _whattorender;
}

void InformationDisplay::SetError(int _whattorendererror) {
	erroractive = true;
	starttime = static_cast<int> (time(NULL));
	whattorendererror = _whattorendererror;
}

void InformationDisplay::ClearAllErrors() {
	erroractive = false;
}

void InformationDisplay::RenderInfo() {
	SDL_RenderCopy(renderer, infotext, &inpos[whattorender], &relpos[whattorender]);
}

void InformationDisplay::RenderError() {
	if (erroractive) {
		if (time(NULL) - starttime <= errordelay) {
			SDL_RenderCopy(renderer, errortext, &inpos[whattorendererror], &errorrelpos);
		} else {
			erroractive = false;
		}
	}
}


InformationDisplay::~InformationDisplay() {}
