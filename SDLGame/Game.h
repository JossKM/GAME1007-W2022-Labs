// Welcome to the lab! Today we will shoot the targets!
// If you need it for reference, the code for spawning asteroids from last week has been pushed to GitHub
// 
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
// -Sorting sprites
// -Bosses etc.
// -Hitpoints/lives for the player
// -Win/Loss/Progression for the game
// -Screen boundaries

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

	float enemySpawnTimer = 0.0f;
	float enemySpawnDelay = 5.0f;

public:
	//Constructor
	Game();
	Game(const char* windowName);

	//Game loop functions
	void run();

	void input();
	void update(const float deltaTime);
	void draw();

	void handlePlayerActions(const float deltaTime);
	void spawnEnemy();
	void updateCollisionDetection();
	void destroyIfOutOfBounds(Sprite* pSpriteToDestroy);

	void quit();

	void cleanup();
};

