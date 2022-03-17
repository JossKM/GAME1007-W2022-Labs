#include "Bullet.h"

Bullet::Bullet(SDL_Renderer* renderer, Vector2 initialVelocity) : Sprite(renderer, "Assets/Lasers/laserBlue16.png")
{
	velocity = initialVelocity;
}