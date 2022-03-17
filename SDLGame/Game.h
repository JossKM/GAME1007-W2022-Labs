//////
//
// Welcome to the lab! Today we will add more sprites and shooting mechanics to the game
// The lab begins at 10:10. Please be prepared with your projects
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
#include <vector> // container class

class Game
{
	int windowSizeX = 800; // Declaring an int and assigning it the value 800
	int windowSizeY = 1000;
	const char* windowName = "Hello SDL";
	SDL_Window* pWindow = nullptr; // Declaring a pointer to SDL_Window and assigning it the value nullptr
	SDL_Renderer* pRenderer = nullptr;
	bool bRunning = false;

	Sprite anotherShip;
	std::vector<Sprite*> sprites; // List of all the things that need to be updated and drawn. (Pointers to objects)

	float bulletSpeed = 500.0f;
	float delayBetweenShots = 0.1f;
	float timeUntilNextShot = 0.0f; // use this as a timer to determine when the next shot can be fired

	bool isShootPressed = false;
	bool isUpPressed = false;
	bool isRightPressed = false;
	bool isDownPressed = false;
	bool isLeftPressed = false;

public:
	//Constructor
	Game();
	Game(const char* windowName);

	//Game loop functions
	void run();

	void input();
	void update(const float deltaTime);
	void draw();

	void quit();

	void cleanup();
};

