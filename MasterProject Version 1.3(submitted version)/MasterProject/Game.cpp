#include "Game.h"
#include "SDL_image.h"
#include "rain.h"
#include "player.h"
#include <vector>
#include <iostream>
#include <SDL_ttf.h>
#include "Text.h"
#include "time.h"
#include <SDL_mixer.h>

//constructer used to set values of some variables and call the startPos function of the player class entity
//this means an instance of game class is made in main, the player's position will be set before the game loop/render functions.
game::game()
{
	player1.startPos(SCREEN_WIDTH, SCREEN_HEIGHT);
	score = 0;
	deathRect = { 0,0, 640, 480 };
	startRect = { 0,0, 640, 480 };
	srand(time(0));
}

//initialise function, initialises SDL and creates window and renderer then assigns surface of window. 
//the if statements are used as checks to see that everything is carried out and then tells it to proceed to the next step.
bool game::init()
{
	bool success = true;
	//if statement checking condition of initialisation.
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else //if fail condition above is false then continue to initalise SDL, cerate window and renderer.
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		gWindow = SDL_CreateWindow("Ame Ame Ame", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);//creating window
		renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);//creating renderer

		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		
	}
	TTF_Init();//initialising SDL ttf functions
	Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 2048);
	int imgflags = IMG_Init(IMG_INIT_PNG); //initalising SDL_Image functions

	return success;//return value for bool type function. I chose bool because this function only needs two returns, the return will only be changed to false if an error occurs.
}

void game::render()
{
	if (delay < 500 && check != true)//for start function, renders help screen, when delay is greater than 500 it sets check to true which makes the render move on to the next else if
	{
		SDL_RenderCopy(renderer, startTexture, NULL, &startRect);
		delay++;
		if (delay >= 500)
		{
			check = true;
		}

	}
	else if (check == true && death == false)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//black
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, playerTexture, NULL, &player1.rect);
		SDL_RenderCopy(renderer, scoreText.texture, NULL, &scoreText.rect);
		for (const auto &enemy : entities)
		{
			SDL_RenderCopy(renderer, rainTexture, NULL, &enemy.rect);
		}
	}

	if (check == true && death == true)
	{
		SDL_RenderCopy(renderer, deathTexture, NULL, &deathRect);//make it say if you want to respawn press space
		Mix_HaltMusic();//stop music
		Mix_HaltChannel(1);//stop channel 1(the channel that plays moveSound)
	}

	SDL_RenderPresent(renderer);
}

void game::update()
{
	player1.screenBoundry(SCREEN_WIDTH);
	scoreText.textRender(std::to_string(score / 9), renderer);
	render();
	rainFallingMotion();
}

bool game::close()
{
	SDL_DestroyWindow(gWindow);
	SDL_DestroyTexture(playerTexture);//destroys playerTexture
	SDL_DestroyTexture(rainTexture);//destroys enemyTexture
	SDL_DestroyTexture(scoreText.texture);//destroys scoreText.texture
	SDL_DestroyTexture(startTexture);//destroys startTexture
	SDL_DestroyTexture(deathTexture);//destroys deathTexture
	Mix_FreeMusic(music);//frees music
	Mix_FreeChunk(moveSound);//frees moveSound
	Mix_FreeChunk(deathSound);//frees deathSound
	Mix_Quit();//quits SDL mixer
	TTF_Quit();//quits SDL ttf
	IMG_Quit();//quits SDL image
	SDL_Quit();//quits SDL
	return true;
}

SDL_Texture *game::loadTexture(std::string mypath)
{
	SDL_Surface* image = IMG_Load(mypath.c_str());
	SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);//free's surface of image after every call

	return imageTexture;
}

bool game::loadImage()
{
	playerTexture = loadTexture("kanna3.png");

	rainTexture = loadTexture("rain.png");


	startTexture = loadTexture("kobayashiStart4.png");

	deathTexture = loadTexture("fafnirDeath2.png");

	return true;
}

void game::loadMusic()
{
	music = Mix_LoadMUS("song2.mp3");
	moveSound = Mix_LoadWAV("kannaSoundEffect3.wav");
	deathSound = Mix_LoadWAV("darkSoulsDeath.wav");
}

bool game::detectCollision(const SDL_Rect &playerRect, const SDL_Rect &enemyRect)
{
	int leftPlayer = playerRect.x;//calculating position of left side of player
	int rightPlayer = playerRect.x + playerRect.w;//calculating position of right side of player
	int topPlayer = playerRect.y;//calculating position of top of player

	int leftBlock = enemyRect.x + 5;//calculating position of left side of block //ADDED VALUE 5 TO MAKE COLLISIONS HARDER(make it easier for the player to avoid)
	int rightBlock = enemyRect.x + enemyRect.w - 5;//calculating position of right side of block  //MINUS VALUE 5 TO MAKE COLLISIONS HARDER(make it easier for the player to avoid)
	int bottomBlock = enemyRect.y + enemyRect.h;//calculating position of bottom of block

	//if statements check if they overlap
	if (topPlayer >= bottomBlock)
	{
		return false;
	}
	if (leftPlayer >= rightBlock)
	{
		return false;
	}
	if (rightPlayer <= leftBlock)
	{
		return false;
	}
	  

	return true;//this function is set as a boolean as it will be inside another function where it it called with the parameters and then called within the loop as the condition in a if statement that then tells the program to call another funtion that resets the positions of player.
}

bool game::detectCollisionCheck()
{
	for (const auto &enemy : entities)
	{
		if (detectCollision(player1.rect, enemy.rect))
			return true;
	}

	return false;
}


void game::createRain()
{
	entities.push_back(rain({ rand() % 615, -1000, 45, 45 }, 7));//pushes a vector back for each type class made(whenever this function is called), then setting the rain rect values and speed; - entities.push_back(blocks({x,y,w,h},speed))
	//set the y position to -1000 to let the player have more time before they enter the screen(gives player time to see/test controls)
}

//function for rains falling and border logic.
void game::rainFallingMotion()
{
	for (auto &enemy : entities)//refers to each element of entities
	{
		enemy.rect.y += enemy.velocity;//make the y value of rect = the y value + the velocity value.(for each entities, increase y value by velocity)
				
		if (enemy.rect.y >= SCREEN_HEIGHT + 40)//border check(+ 40 are the amount of pixels it can move off the canvas, meaning it has a small delay before the motion resets)
		{
			enemy.rect.y = 0;//reset
			enemy.rect.x = rand() % 615;//is the screen width - half of the rect's width(rect's width is 50) 
			//[the x position of the enemies will be randomised on reset between 0 and 615 meaning it cannot spawn at the the screen width meaning it will appear off canvas]
			score += 1;//keeps track of score, for every wave that passes you earn 1 score.
		}
	}
}

void game::start()
{
	bool completed = false;//create bool for while loop
	while (!completed)//when started is false call render, in render there is an if statement meaning only 1 thing will render
	{
		render();

		if (check == true)//when the condition is met in the render if statement, check becomes true meaning this if statement will be run which sets the while loop condition as true meaning the while loop will end.
		{
			completed = true;
		}
	}
	
}

bool game::loop()
{

	bool isRunning = true; //bool for while loop
	SDL_Event keyEvent;
	loadMusic();
	Mix_PlayMusic(music, -1);//music will play when the loop is called

	//calling function that creates an enemy
	createRain();
	createRain();
	createRain();
	createRain();
	createRain();
	createRain();
	createRain();
	createRain();
	createRain();

	

	while (isRunning)
	{
		
		while (SDL_PollEvent(&keyEvent))
		{
			if (keyEvent.type == SDL_QUIT)
			{
				isRunning = false;
			}
			else if (keyEvent.type == SDL_KEYDOWN)
			{
				switch (keyEvent.key.keysym.sym)
				{
				case SDLK_p:
					if (!Mix_PlayingMusic())
					{
						Mix_PlayMusic(music, -1);//if music is not currently playing play music when p is pressed
					}
					break;
				case SDLK_m:
					Mix_HaltMusic();
					break;
				case SDLK_RIGHT:
					if (!Mix_Playing(-1))
						Mix_PlayChannel(1, moveSound, 0);
					break;
				case SDLK_LEFT:
					if (!Mix_Playing(-1))
						Mix_PlayChannel(1, moveSound, 0);
					break;
					
				}
			}
				
		}
		keyState = SDL_GetKeyboardState(NULL);
		if (keyState[SDL_SCANCODE_RIGHT])
		{
			player1.rect.x += player1.speed;
		}

		if (keyState[SDL_SCANCODE_LEFT])
		{
			player1.rect.x -= player1.speed;
		}

		if (detectCollisionCheck())
		{
			player1.offScreen();
			death = true;
			Mix_PlayChannel(2, deathSound, 0);//plays sound upon death(collision).
		}
		update();
		SDL_Delay(16);
	}

	return true;
}