#include "Sprite.h"
#include <minmax.h>

//Constructors define what to do when creating an object of this class type
Sprite::Sprite()
{
	pImage = nullptr;
	rotation = 0;
	dst.x = 0;
	dst.y = 0;
	dst.w = 0;
	dst.h = 0;
	src.x = 0;
	src.y = 0;
	src.w = 0;
	src.h = 0;
}

Sprite::Sprite(SDL_Renderer* pRenderer, const char* textureFilename)
{
	rotation = 0;
	dst.x = 0;
	dst.y = 0;
	pImage = IMG_LoadTexture(pRenderer, textureFilename);
	SDL_QueryTexture(pImage, NULL, NULL, &dst.w, &dst.h);

	src.x = 0;
	src.y = 0;
	src.w = dst.w;
	src.h = dst.h;
}

Sprite::~Sprite()
{
}

void Sprite::update(const float deltaTime)
{
	//displacement is a change in position
	//velocity is defined as displacement / time,
	//if we multiply velocity by time we get displacement
	Vector2 displacement =
	{
		velocity.x * deltaTime,
		velocity.y * deltaTime
	};

	//add displacement to position
	position.x = position.x + displacement.x;
	position.y = position.y + displacement.y;

	dst.x = position.x + 0.5f; // add 0.5 if you want to round up/down when converting float to int
	dst.y = position.y + 0.5f; // add 0.5 if you want to round up/down when converting float to int
}

void Sprite::draw(SDL_Renderer* pRenderer)
{
	SDL_RenderCopyEx(pRenderer, pImage, &src, &dst, rotation, nullptr, SDL_FLIP_NONE); // Draw our texture at the destination position, width, and size
}

Vector2 Sprite::getSize()
{
	return Vector2{(float)dst.w, (float)dst.h};
}

void Sprite::setPosition(int x, int y)
{
	dst.x = x;
	dst.y = y;
	position.x = x;
	position.y = y;
}

void Sprite::moveBy(int x, int y)
{
	dst.x += x;
	dst.y += y;
	position.x += x;
	position.y += y;
}

void Sprite::setDimensions(int width, int height)
{
	dst.w = width;
	dst.h = height;
}

void Sprite::setRotation(double rotationDegrees)
{
	rotation = rotationDegrees;
}

void Sprite::cleanup()
{
	SDL_DestroyTexture(pImage);
}

bool Sprite::isCollidingWith(Sprite* other)
{
	//1. Find the distance between this sprite and the other
	//1.a. Find a vector from this sprite to the other using vector subtraction
	Vector2 displacementBetween =
	{
		other->position.x - position.x,
		other->position.y - position.y
	};

	//1.b. Find the length of that vector using pythagorean theorem
	//square sides, add, take square root
	float distance = sqrt(pow(displacementBetween.x,2) + pow(displacementBetween.y, 2));

	//2. Find the sum of the radii of circles surrounding these sprites
	//2.a find an appropriate radius for each sprite based on their size?
	float myRadius = max(getSize().x, getSize().y);
	float otherRadius = max(other->getSize().x, other->getSize().y);
	//2.b. add them
	float sumRadii = myRadius + otherRadius;

	//3. Check if the distance is less than the sum of the radii. If so, they are colliding. If not, they are not.
	if (distance < sumRadii)
	{
		return true; // Collision!
	}
	else
	{
		return false;
	}
}
