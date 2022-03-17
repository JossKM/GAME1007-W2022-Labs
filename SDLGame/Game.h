//////
//
// Welcome to the lab! Today we will add more sprites and shooting mechanics to the game
// The lab begins at 12:10. Please be prepared with your projects
// If you need it for reference, the code for SDL setup has been pushed to GitHub
// 
// Quiz 7 available
// 
// At the end of all the labs... we will do..
// -Basic collision detection between sprites
// -Hold space to shoot
// -Spawn targets to shoot
// -Detect when player touches a target
// -Detect when player bullet touches a target
// -Play music and sounds
// 
// Will NOT show you in the labs
// -How to make enemies shoot back
// -Infinitely scrolling background
// -Sorting
// -Screen boundaries
//////

#pragma once
#include <SDL.h> 
#include "Sprite.h"

class Game
{
	int windowSizeX = 800; // Declaring integer called windowSizeX, assigning it the value 800
	int windowSizeY = 600;
	const char* windowName = "Hello SDL";
	SDL_Window* pWindow = nullptr; // Declaring a SDL_Window* (pointer to an object of type SDL_Window) called pWindow, assigning it the value nullptr
	SDL_Renderer* pRenderer = nullptr;
	bool bIsRunning = false;

	Sprite myShip;
	Sprite myBullet;
	Sprite myBackground;

	float gameTime = 0.0f; // seconds since start of game
	float fixedDeltaTime = 0.016f; // time between frames 0.016 == 1/60

	bool isUpPressed	= false;
	bool isDownPressed	= false;
	bool isLeftPressed	= false;
	bool isRightPressed	= false;

public:
	Game();

	int run();
	void input();
	void update(const float deltaTime);
	void draw();
	void quit();
	void cleanup();
};

