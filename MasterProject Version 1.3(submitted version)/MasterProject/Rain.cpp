#include <SDL.h>
#include "rain.h"




rain::rain(SDL_Rect enemy, int speed)
{
	rect = enemy;
	velocity = speed;
}