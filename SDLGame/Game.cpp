#include "Game.h"
#include <SDL_image.h>
#include <iostream>
#include "Bullet.h"

Game::Game()
{

}

Game::Game(const char* windowName)
{
	int flags = SDL_INIT_EVERYTHING;
	int rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC; //These are two integers representing settings combined with the | operator. This pattern you will see in other cases too

	//Getting SDL features booted up
	if (SDL_Init(flags) == 0) // if initialized SDL correctly...
	{
		// Create the window
		pWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSizeX, windowSizeY, SDL_WINDOW_SHOWN);

		if (pWindow) // If window was created successfully
		{
			//Create SDL_Renderer object
			pRenderer = SDL_CreateRenderer(pWindow, -1, rendererFlags);

			if (pRenderer)
			{
				IMG_Init(IMG_INIT_PNG);
			}
		}
	}
}

void Game::run()
{
	bRunning = true;

	anotherShip = Sprite(pRenderer, "Assets/playerShip1_red.png");
	anotherShip.setPosition((windowSizeX*0.5) - (anotherShip.getSize().x * 0.5f), windowSizeY - (anotherShip.getSize().y) - 50);
	anotherShip.setRotation(0);
	sprites.push_back(&anotherShip); // & gives the address of the object anotherShip

	float fixedDeltaTimeSeconds = 0.016; // seconds

	//Really really basic game loop... forever
	while (bRunning)
	{
		input();

		update(fixedDeltaTimeSeconds);

		draw();

		SDL_Delay(fixedDeltaTimeSeconds * 1000); // Wait 16 ms between frames
	}

	cleanup();
}

void Game::input()
{
	//SDL's input system works by "polling" devices
	SDL_Event lastInput;

	// & is the 'reference' operator. 
	//It gets the address of a variable i.e. &lastInput means "memory address of variable lastInput"
	while(SDL_PollEvent(&lastInput))
	{
		//pressing a button starts with going from up to down, then held, then down to up
		if (lastInput.type == SDL_KEYDOWN)
		{
			switch (lastInput.key.keysym.sym)
			{
			case(SDLK_SPACE):
				isShootPressed = true;
				break;
			case(SDLK_w):
				//std::cout << "Up" << std::endl;
				isUpPressed = true;
				break;
			case(SDLK_d):
				//std::cout << "Right" << std::endl;
				isRightPressed = true;
				break;
			case(SDLK_s):
				//std::cout << "Down" << std::endl;
				isDownPressed = true;
				break;
			case(SDLK_a):
				//std::cout << "Left" << std::endl;
				isLeftPressed = true;
				break;
			}
		} else if (lastInput.type == SDL_KEYUP)
		{
			switch (lastInput.key.keysym.sym)
			{
			case(SDLK_SPACE):
				isShootPressed = false;
				break;
			case(SDLK_w):
				isUpPressed = false;
				break;
			case(SDLK_d):
				isRightPressed = false;
				break;
			case(SDLK_s):
				isDownPressed = false;
				break;
			case(SDLK_a):
				isLeftPressed = false;
				break;
			}
		}
	}
}



void Game::update(const float deltaTime)
{
	//Check collision between bullets and asteroids
	updateCollisionDetection();

	enemySpawnTimer -= deltaTime;
	///Spawn asteroids periodically
	if (enemySpawnTimer <= 0)
	{
		spawnEnemy();
		enemySpawnTimer = enemySpawnDelay;

		if (enemySpawnDelay > 1.0f)
		{
			enemySpawnDelay -= 0.1f;  // increase difficulty?
		}
	}

	handlePlayerActions(deltaTime);

	//update our sprites
	for (int i = 0; i < sprites.size(); i++)
	{
		Sprite* pSprite = sprites[i];
		pSprite->update(deltaTime);

		//Destroy bullets out of bounds
		Bullet* pBullet = dynamic_cast<Bullet*>(pSprite); // Casting a base class pointer to a child class (not always successful)
		//If unsuccessfule (i.e. it's not a bullet) then the pointer after the cast will be null
		destroyIfOutOfBounds(pBullet);
	}

	//Clean up dead sprites. Removes them from the container if they are marked for deletion
	for (auto iterator = sprites.begin(); iterator != sprites.end();)
	{
		Sprite* pSprite = (*iterator);
		if (pSprite->getMarkedForDeletion())
		{
			pSprite->cleanup();
			delete pSprite; // clears the memory
			pSprite = nullptr;
			iterator = sprites.erase(iterator); // remove element from container
		}
		else
		{
			iterator++;
		}
	}
}

void Game::draw()
{
	SDL_SetRenderDrawColor(pRenderer, 245, 180, 180, 255); // Select a color
	SDL_RenderClear(pRenderer); // Paint the canvas according to the last selected color

	//Draw all the things
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->draw(pRenderer);
	}

	SDL_RenderPresent(pRenderer); // Show the canvas
}

void Game::handlePlayerActions(const float deltaTime)
{/// <summary>
	///  Player actions
	/// </summary>
	/// <param name="deltaTime"></param>
	float acceleration = 3500.0f;
	float deltaV = acceleration * deltaTime;

	timeUntilNextShot = (timeUntilNextShot - deltaTime);
	if (timeUntilNextShot < 0.0f)
	{
		timeUntilNextShot = 0.0f;
	}

	if (isShootPressed)
	{
		if (timeUntilNextShot <= 0.0f)
		{
			int numProjectiles = 3;
			float totalSpread = 1.0f;
			for (int i = 0; i < numProjectiles; i++)
			{
				float angle = (i * (totalSpread / (numProjectiles - 1))) - (totalSpread * 0.5);
				Vector2 launchVector = Vector2{ sin(angle) * -bulletSpeed, cos(angle) * -bulletSpeed };

				Bullet* newBullet = new Bullet(pRenderer, launchVector); // the new keyword creates an instance of the class and returns a pointer to it
				//Danger of the new keyword is that we are responsible for releasing the memory allocated for this object with the keyword delete

				//the -> operator gets you the object the pointer points at
				newBullet->position.x = (anotherShip.position.x + (anotherShip.getSize().x * 0.5f)) - (newBullet->getSize().x * 0.5f);
				// a->b() operator is actually a shortcut for (*a).b() which is derefencing the pointer, and then using the . operator on the object
				(*newBullet).position.y = anotherShip.position.y;

				Sprite* castedPointer = (Sprite*)newBullet; // Polymorphism allows the inheriting class "Bullet" to be treated like a "Sprite"
				//Because a Bullet IS a Sprite
				sprites.push_back(castedPointer); // Assigning a pointer 
			}
			timeUntilNextShot = delayBetweenShots;
		}
	}
	if (isUpPressed)
	{
		anotherShip.velocity.y -= deltaV;
	}
	if (isRightPressed)
	{
		anotherShip.velocity.x += deltaV;
	}
	if (isDownPressed)
	{
		anotherShip.velocity.y += deltaV;
	}
	if (isLeftPressed)
	{
		anotherShip.velocity.x -= deltaV;
	}

	float damping = 56.25f * deltaTime;
	anotherShip.velocity.x *= damping;
	anotherShip.velocity.y *= damping;
}

void Game::spawnEnemy()
{
	const int NUM_SPRITES = 3;
	std::string enemySprites[NUM_SPRITES] = {
		"Assets/Meteors/meteorBrown_big1.png",
		"Assets/Meteors/meteorBrown_big2.png",
		"Assets/Meteors/meteorBrown_big3.png"
	};
	const char* spriteFileToChoose = (enemySprites[rand() % NUM_SPRITES]).c_str(); //Choose a random sprite
	Sprite* newEnemy = new Sprite(pRenderer, spriteFileToChoose);
	newEnemy->tag = ASTEROID;

	//Spawn near top of screen
	int xStart = (rand() % windowSizeX) - (newEnemy->getSize().x);
	int yStart = (rand() % 10) - newEnemy->getSize().y;
	newEnemy->setPosition(xStart, yStart);

	newEnemy->velocity = Vector2{float((rand() % 200) - 100), float((rand() % 100) + 100.0f)};

	sprites.push_back(newEnemy);
}

void Game::updateCollisionDetection()
{
	//Write this function (with actual code) and send it to me now (just copy paste it into a private message)
	//Check collision between bullets and asteroids
	//For each bullet...
	//	For each asteroid...
	//		if bullet touching asteroid (using our Sprite isColliding() function)...
	//			then make the asteroid disappear... or more...?
	//		else
	//			do nothing...
	//

	//we could restructure the code to have a container (std::vector) of just bullets and just asteroids
	//we could visit each sprite and find out if it is a bullet?
	//we could just check every sprite against every other...
	//For each sprite
		//For each other sprite
			//if one is a bullet and one is an asteroid...
				//if they are colliding
					//destroy both
				//else do nothing

	for (int i = 0; i < sprites.size(); i++)
	{
		Sprite* pSprite1 = sprites[i];

		for (int j = i + 1; j < sprites.size(); j++)
		{
			Sprite* pSprite2 = sprites[j];

			//how can i tell if they are bullet and asteroid???
			if (pSprite1->tag == BULLET && pSprite2->tag == ASTEROID 
				|| pSprite2->tag == BULLET && pSprite1->tag == ASTEROID)
			{
				if (pSprite1->isCollidingWith(pSprite2))
				{
					pSprite1->destroy();
					pSprite2->destroy();
				}
			}
		}
	}
}

void Game::destroyIfOutOfBounds(Sprite* pSpriteToDestroy)
{
	if (pSpriteToDestroy != nullptr) // success. Must be a bullet...
	{
		if (pSpriteToDestroy->position.y > windowSizeY
			|| pSpriteToDestroy->position.y < -pSpriteToDestroy->getSize().y
			|| pSpriteToDestroy->position.x < -pSpriteToDestroy->getSize().x
			|| pSpriteToDestroy->position.x > windowSizeX
			)
		{
			pSpriteToDestroy->destroy();
		}
	}
}

void Game::quit()
{
	bRunning = false;
}

void Game::cleanup()
{	
	anotherShip.cleanup();
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
}
