#pragma once
#include <SDL.h>
#include "SDL_ttf.h"
#include <string>

class text
{
private:
	TTF_Font *font;
	SDL_Color colour;
public:
	text();

	SDL_Texture *texture;
	SDL_Rect rect;

	SDL_Texture* textRender(const std::string &message, SDL_Renderer *renderer);

};