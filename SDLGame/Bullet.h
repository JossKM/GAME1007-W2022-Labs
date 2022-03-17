#pragma once
#include "Sprite.h"

class Bullet : public Sprite // this is 'public inheritance'. Bullet will inherit all of the features of Sprite
{
public:
	Bullet(SDL_Renderer* renderer, Vector2 initialVelocity); //Constructor for bullet
};

