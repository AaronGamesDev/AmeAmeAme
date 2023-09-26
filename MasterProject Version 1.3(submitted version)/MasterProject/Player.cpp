#include "Player.h"
#include <string>
#include <iostream>


player::player()
{
	rect.w = 40;
	rect.h = 60;
}

player::~player()
{

}

void player::screenBoundry(int screenWidth)
{

	if (rect.x < 0)
	{
		rect.x = screenWidth - rect.w;
	}

	if (rect.x + rect.w > screenWidth)
	{
		rect.x = 0;
	}


}

bool player::startPos(int screenWidth, int screenHeight)
{
	rect.x = (screenWidth/2) - (rect.w/2);
	rect.y = screenHeight - rect.h;

	return true;
}

bool player::offScreen()
{
	rect.x = 800;
	rect.y = 600;
	return true;
}