#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include "Math.hpp"

class Entity
{
public:
	Entity(Vector2f p_pos, SDL_Texture* p_tex, int width, int height);
	Vector2f& getPos()
	{
		return pos;
	}
	SDL_Texture* getTex();
	SDL_Rect getCurrentFrame();
	void setPos(int x, int y);
	Vector2f getMid();
	int getFeetY();
	Vector2f getFeetX();
private:
	Vector2f pos;
	SDL_Rect currentFrame;
	SDL_Texture* tex;
};