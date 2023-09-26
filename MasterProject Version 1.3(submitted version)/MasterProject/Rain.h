#pragma once
#include <SDL.h>
#include <vector>

class rain
{
public:
	rain(SDL_Rect enemy, int velocity);//constructer sets values
	SDL_Rect rect;
	int velocity;

};