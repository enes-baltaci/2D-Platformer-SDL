#include "Entity.hpp"

Entity::Entity(Vector2f p_pos, SDL_Texture* p_tex, int width, int height)
	:pos(p_pos), tex(p_tex)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = width;
	currentFrame.h = height;
}

SDL_Texture* Entity::getTex()
{
	return tex;
}

SDL_Rect Entity::getCurrentFrame()
{
	return currentFrame;
}

void Entity::setPos(int x, int y)
{
	pos.x = x;
	pos.y = y;
}

Vector2f Entity::getMid()
{
	return (Vector2f { pos.x + currentFrame.w * 2, pos.y + currentFrame.h * 2 });
}

int Entity::getFeetY()
{
	return ( (pos.y + currentFrame.h * 4) - 56 );
}

Vector2f Entity::getFeetX()
{
	return (Vector2f{ pos.x + 92, pos.x + 160 });
}