#include <SDL.h>
#include "SDL_image.h"
#include <string>
#include "game.h"


int main(int argc, char** argv)
{	
	game Game;
	Game.init();
	Game.loadImage();
	Game.start();
	Game.loop();
	Game.close();
	
	return 1;
}