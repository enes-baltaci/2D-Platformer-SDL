#include "Entity.hpp"

Entity::Entity(Vector2f p_pos, SDL_Texture* p_tex)
	:pos(p_pos), tex(p_tex)
{
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 32;
	currentFrame.h = 32;
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