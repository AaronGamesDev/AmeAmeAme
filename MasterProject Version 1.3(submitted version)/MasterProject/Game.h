#pragma once
#include <SDL.h>
#include "player.h"
#include <string>
#include "rain.h"
#include <vector>
#include "Text.h"
#include <SDL_mixer.h>

class game
{
private:
	SDL_Window* gWindow = NULL;
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	SDL_Texture *playerTexture;
	SDL_Texture *rainTexture;

	SDL_Renderer *renderer;
	
	SDL_Texture* startTexture;
	SDL_Texture* deathTexture;
	SDL_Rect startRect;
	SDL_Rect deathRect;

	player player1;
	std::vector<rain> entities;
	int score;
	bool death = false;

	const Uint8 *keyState;

	text scoreText;
	int delay = 1;
	bool check = false;

	Mix_Music *music;
	Mix_Chunk *moveSound;
	Mix_Chunk *deathSound;

public:
	game();
	SDL_Texture* loadTexture(std::string path);
	bool loadImage();
	void render();
	bool init();
	bool close();
	void update();
	bool loop();
	bool detectCollision(const SDL_Rect &player, const SDL_Rect &enemy);//parameters are set as const so that they cannot be changed within the function.
	bool detectCollisionCheck();
	void createRain();
	void rainFallingMotion();
	void loadMusic();
	void start();
};