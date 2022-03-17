#include "Game.h"
#include <SDL_image.h>
#include <iostream>
#include "Bullet.h"

Game::Game()
{
}

int Game::run()
{
	// Setting up SDL features...
	int flags = SDL_INIT_EVERYTHING;
	if (SDL_Init(flags) != 0) // if initialized SDL correctly...
	{
		std::cout << "SDL failed to initialize" << std::endl;
		return -1;
	}

	// Create the window
	pWindow = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSizeX, windowSizeY, SDL_WINDOW_SHOWN);
	if (pWindow == NULL) // If 
	{
		std::cout << "window failed to create" << std::endl;
		return -1;
	}

	// The renderer is needed to draw to the screen
	pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (pRenderer == NULL)
	{
		std::cout << "renderer failed to create" << std::endl;
		return -1;
	}

	int whatWasInitialized = IMG_Init(IMG_INIT_PNG);
	if (whatWasInitialized != IMG_INIT_PNG)
	{
		std::cout << "SDL_img failed to initialize" << std::endl;
		return -1;
	}

	myShip = Sprite(pRenderer, "Assets/playerShip3_red.png");
	myShip.setPosition(400, 300);
	//sprites.push_back(&myShip); // & is the reference operator. It will return a pointer to the address of the object to its right

	myBackground = Sprite(pRenderer, "Assets/Backgrounds/purple.png");
	myBackground.setSize(800, 600);

	bIsRunning = true;

	//Main game loop
	while (bIsRunning)
	{
		input();
		
		update(fixedDeltaTime);

		draw();

		SDL_Delay(fixedDeltaTime * 1000); // wait some milliseconds
		gameTime += fixedDeltaTime;
	}

	cleanup();

	return 0;
}

void Game::input()
{
	//SDL uses an event queue. Process the next thing in the queue using SDL_GetEvents
	//To get another event from the queue, use SDL_PollEvent
	SDL_Event lastEvent;

	//& gives the "memory address of" the thing we use it on. So here we give SDLPollEvent the address of our lastEvent variable 
	while(SDL_PollEvent(&lastEvent))
	{
		//Keydown? There are several parts of a button press. pressing down, held, and then letting go
		if (lastEvent.type == SDL_KEYDOWN)
		{
			switch (lastEvent.key.keysym.sym)
			{
			case(SDLK_SPACE):
				isShootPressed = true;
				break;
			case(SDLK_w):
				isUpPressed = true;
				break;
			case(SDLK_d):
				isRightPressed = true;
				break;
			case(SDLK_s):
				isDownPressed = true;
				break;
			case(SDLK_a):
				isLeftPressed = true;
				break;
			}

		} else if (lastEvent.type == SDL_KEYUP)
		{
			switch (lastEvent.key.keysym.sym)
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
	//acceleration = change in velocity over time
	float acceleration = 3000;
	float deltaV = acceleration * deltaTime;
	
	Vector2 inputVector =
	{
		0,
		0
	};

	if (isUpPressed)
	{
		inputVector.y -= 1;
	}
	if (isRightPressed)
	{
		inputVector.x += 1;
	}
	if (isDownPressed)
	{
		inputVector.y += 1;
	}
	if (isLeftPressed)
	{
		inputVector.x -= 1;
	}

	//Normalize the input vector by finding its length...
	//Pythagorean theorem sqrt(x^2 + y^2)
	float length = sqrtf(pow(inputVector.x, 2) + pow(inputVector.y, 2));
	float lenghReciprocal = 1.0f / length;

	//Then dividing it by its length (dividing vector by scalar)
	Vector2 inputVectorNormalized =
	{
		inputVector.x * lenghReciprocal,
		inputVector.y * lenghReciprocal
	};

	//Apply acceleration in the direction of that input vector
	Vector2 finalVectorDeltaV =
	{
		inputVector.x * deltaV,
		inputVector.y * deltaV
	};

	myShip.velocity.x += finalVectorDeltaV.x;
	myShip.velocity.y += finalVectorDeltaV.y;

	float damping = 0.9f / deltaTime;
	myShip.velocity.x *= damping * deltaTime;
	myShip.velocity.y *= damping * deltaTime;

	timeBeforeNextShot -= deltaTime;

	if (isShootPressed)
	{
		if (timeBeforeNextShot <= 0.0f)
		{
			int projectilesPerShot = 3;
			float spread = 1.0f;
			float bulletSpeed = 400.0f;
			for (int i = 0; i < projectilesPerShot; i++)
			{
				float angle = ((spread / projectilesPerShot) * i) + (spread);

				std::cout << "shoot!" << std::endl;
				Bullet* pNewBullet = new Bullet(pRenderer); // the new keyword creates an instance of that class type, and returns a pointer to it
				// The danger of the new keyword is that we are now responsible for deallocating the memory for this object with the keyword delete
				Sprite* pBulletCastedToSprite = (Sprite*)pNewBullet; // cast from child class to base class pointer

				Vector2 launchPosition = Vector2{ myShip.position.x + (myShip.getSize().x * 0.5f) - (pNewBullet->getSize().x * 0.5f),
													myShip.position.y
				};

				Vector2 launchVelocity = Vector2{ cos(angle) * bulletSpeed,
												  sin(angle) * -bulletSpeed
				};

				pNewBullet->position = launchPosition;
				pNewBullet->velocity = launchVelocity;

				sprites.push_back(pBulletCastedToSprite);
			}

			timeBeforeNextShot = timeBetweenShots;
		}
	}

	for (int i = 0; i < sprites.size(); i++)
	{
		Sprite* pSprite = sprites[i];

		if (pSprite->position.x > windowSizeX
			|| pSprite->position.x < pSprite->getSize().x
			|| pSprite->position.y > windowSizeY
			|| pSprite->position.y < -pSprite->getSize().y
			)
		{
			pSprite->isMarkedForDeletion = true;
		}

		pSprite->update(deltaTime);
	}
	myShip.update(deltaTime);

	//Loop to Destroy sprites as needed
	for (auto iterator = sprites.begin(); iterator != sprites.end();)
	{
		//To get an element from an iterator, use the dereference operator *, which usually turns a pointer into an object
		Sprite* pSprite = (*iterator);

		// -> is a shortcut for dereferencing a pointer, then using the "." operator on it,
		// i.e. a->b is equivalent to (*a).b
		if ((*pSprite).isMarkedForDeletion)
		{
			//clear the memory from the image of the sprite!
			pSprite->cleanup();
			// destroy it!
			delete pSprite; // deallocate memory that this pointer points to
			pSprite = nullptr;
			iterator = sprites.erase(iterator); // Remove this element from the container
		}
		else
		{
			iterator++;
		}
	}

	sprites.shrink_to_fit();
}

void Game::draw()
{
	//Render
	SDL_SetRenderDrawColor(pRenderer, 255, 200, 200, 255); // Choose a color
	SDL_RenderClear(pRenderer); // Clear canvas to color chosen

	myBackground.draw(pRenderer);
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->draw(pRenderer);
	}
	myShip.draw(pRenderer);

	SDL_RenderPresent(pRenderer); // Make updated canvas visible on screen
}

void Game::quit()
{
	bIsRunning = false;
}

void Game::cleanup()
{
	// Clean up
	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);

	myShip.cleanup();
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->cleanup();
		delete sprites[i];
	}
	myBackground.cleanup();
}
