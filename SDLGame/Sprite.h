#pragma once
#include <SDL_image.h>
#include "Vector2.h"

// A sprite is any image which lives on the screen and can be moved about
//Sprite should contain functionality to load an image, and have its own position

//It should contain:
// texture (image)
// position
// size
// rotation

//It can do:
// load an image
// draw itself at a location
// set the size of the sprite
// set the position of the sprite
// set the rotation

//Sprite myShip = Sprite("ship.png")
//myShip.setPosition(50, 600) // set position globally on the screen
//myShip.draw(pRenderer)
//myShip.move(2,0) // move horizontally to the right relative to its current position
//myShip.setDimensions(60, 60) //set size in pixels
//myShip.setScale(2.5, 2.5) //scale sprite by 2.5x
//myShip.setRotation(180)

enum GameplayTag
{
	NONE = -1,
	PLAYER,
	ASTEROID,
	BULLET
};

//Declaring a new class called Sprite
class Sprite
{
private:
	//Data members
	SDL_Texture* pImage = nullptr;
	SDL_Rect dst; //destination rectangle, representing position and size of the sprite
	SDL_Rect src; //source rectangle, representing which part of the texture to draw this sprite from
	//a double is short for double-precision floating-point number. float is short for floating point number
	double rotation;
	bool isMarkedForDeletion = false;

public:
	Vector2 position = {0,0};
	Vector2 velocity = {0,0};
	GameplayTag tag = NONE;

	//Constructors
	Sprite(); // default constructor
	Sprite(SDL_Renderer* pRenderer, const char* textureFilename); // constructor with parameters for the image file
	virtual ~Sprite(); // The keyword "virtual" has to do with a cool feature of programming languages called "polymorphism". 
	//marking a function virtual means it can be "overridden" by a class that inherits from this class

	//Updates position based on position and velocity vectors
	virtual void update(const float deltaTime);

	//Declaring Member functions <return type><name>(<parameters>)
	virtual void draw(SDL_Renderer* pRenderer);

	//getters, also called "accessors"
	Vector2 getSize();
	bool getMarkedForDeletion();

	//setters, also called "modifiers" or "mutators"
	void setPosition(int x, int y);
	void moveBy(int x, int y);
	void setDimensions(int width, int height);
	void setRotation(double rotationDegrees);

	void destroy();
	void cleanup();

	//getters
	SDL_Point getPosition();

	// It is a good idea to make sure you design code to be easily read like regular human language
	// consider:
	// CollisionManager::CircleCircleCollisionDetect(circle1, circle2) 
	// ship->isCollidingWith(Asteroid1)
	// 
	// ship = new Sprite(500, 670, 30, 60);
	// ship.setPosition(500, 670);
	// ship.setDimensions(30, 60);
	bool isCollidingWith(Sprite* other); // return true if colliding (circle-circle) and false otherwise
};

