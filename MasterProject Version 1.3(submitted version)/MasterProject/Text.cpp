#include "Text.h"
#include <SDL_ttf.h>

text::text()
{
	TTF_Init();
	font = TTF_OpenFont("Otterly Adorable - TTF.ttf", 30);
	colour = { 227, 0, 0, 255 };
}

SDL_Texture* text::textRender(const std::string &message, SDL_Renderer *renderer) 
{
	SDL_DestroyTexture(texture);
	SDL_Surface* Surface = TTF_RenderText_Solid(font, message.c_str(), colour);
	texture = SDL_CreateTextureFromSurface(renderer, Surface);
	SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);



	SDL_FreeSurface(Surface);
	return texture;
}
