#pragma once
#include <SDL.h>


class player
{

public:
	const int speed = 5;
	SDL_Rect rect;
	player();
	~player();
	void screenBoundry(int screenWidth);
	bool startPos(int screenWidth, int screenHeight); //was going to be used in for respawn however removed feature.
	bool offScreen();
};