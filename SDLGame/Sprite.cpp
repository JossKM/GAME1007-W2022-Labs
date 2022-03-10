#include "Sprite.h"

Sprite::Sprite()
{
	pTexture = nullptr;
	src.x = 0;
	src.y = 0;
	src.w = 0;
	src.h = 0;

	dst.x = 0;
	dst.y = 0;
	dst.w = 0;
	dst.h = 0;
}

Sprite::Sprite(SDL_Renderer* pRenderer, const char* filename) : Sprite()
{
	pTexture = IMG_LoadTexture(pRenderer, filename);
	SDL_QueryTexture(pTexture, NULL, NULL, &dst.w, &dst.h);
	src.w = dst.w;
	src.h = dst.h;
}

void Sprite::setPosition(const float x, const float y)
{
	dst.x = x;
	dst.y = y;
	position.x = x;
	position.y = y;
}

void Sprite::moveBy(const float xOffset, const float yOffset)
{
	dst.x += xOffset;
	dst.y += yOffset;
	position.x += xOffset;
	position.y += yOffset;
}

void Sprite::setSize(int width, int height)
{
	dst.w = width;
	dst.h = height;
}

void Sprite::update(const float deltaTime)
{
	//Displacement is a change in position
	//Velocity is a displacement over time 
	// calculate displacment with vector multiplication by a scalar (velocity * time)
	Vector2 displacement =
	{
		 velocity.x * deltaTime,
		 velocity.y * deltaTime
	};

	//Use velocity and time to update our sprite position using vector addition (position + displacement)
	position.x += displacement.x;
	position.y += displacement.y;

	dst.x = position.x;
	dst.y = position.y;
}

void Sprite::draw(SDL_Renderer* pRenderer) const
{
	SDL_RenderCopy(pRenderer, pTexture, &src, &dst); // Draw texture
}

void Sprite::cleanup()
{
	SDL_DestroyTexture(pTexture);
}
