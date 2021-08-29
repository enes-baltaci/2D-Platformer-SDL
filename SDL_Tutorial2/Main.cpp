#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <Windows.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Utils.hpp"

void setCursor(int x, int y) // copy paste from stackoverflow
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hidecursor() // copy paste from stackoverflow
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

int main(int argc, char* args[])
{
	hidecursor();

	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "SDL_Init has failed." << SDL_GetError() << std::endl;

	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed." << SDL_GetError() << std::endl;

	RenderWindow window("Game v1.0", 1280, 720);

	SDL_Texture* knight = window.loadTexture("hulking_knight.png");

	SDL_Texture* platform = window.loadTexture("2dplatform.png");

	std::vector<Entity> entitiees = { Entity(Vector2f(50, 0), knight, 64, 60),
									  Entity(Vector2f(0, 630), platform, 320, 25), };

	bool gameRunning = 1, mouse = 0, down = 0;

	SDL_Event event;

	const float timeStep = 0.01f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();

	while (entitiees[0].getMid().y != 550)
	{
		window.clear();

		window.render(entitiees[0]);
		window.render(entitiees[1]);

		window.display();

		SDL_Delay(1);

		entitiees[0].setPos(entitiees[0].getPos().x, entitiees[0].getPos().y + 1);
	}

	while (gameRunning)
	{
		int startTicks = SDL_GetTicks(), mouseX, mouseY;

		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;

		currentTime = newTime;

		accumulator += frameTime;

		while (accumulator >= timeStep)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					gameRunning = 0;

				if (!mouse && (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP))
				{
					mouse = 1;
				}
			}

			accumulator -= timeStep;
		}

		const float alpha = accumulator / timeStep;

		window.clear();

		if (mouse)
		{
			if (entitiees[0].getPos().y > 250 && !down)
			{
				entitiees[0].setPos(entitiees[0].getPos().x, entitiees[0].getPos().y - 10);
			}
			else
			{
				entitiees[0].setPos(entitiees[0].getPos().x, entitiees[0].getPos().y + 10);
				down = 1;
			}

			if (entitiees[0].getPos().y == 430)
			{
				mouse = 0;
				down = 0;
			}
		}

		window.render(entitiees[0]);
		window.render(entitiees[1]);

		setCursor(0, 0);
		std::cout << utils::hireTimeInSeconds() << std::endl;

		window.display();

		SDL_GetMouseState(&mouseX, &mouseY);
		setCursor(0, 2);
		std::cout << mouseX << ", " << mouseY << std::endl;

		if (entitiees[0].getMid().x - mouseX < 150 && entitiees[0].getMid().x - mouseX > 0 && mouseY - entitiees[0].getPos().y < 200 && mouseY - entitiees[0].getPos().y > 0)
		{
			entitiees[0].setPos(entitiees[0].getPos().x + 5, entitiees[0].getPos().y);
		}

		int frameTicks = SDL_GetTicks() - startTicks;

		if (frameTicks < 1000 / window.getRefreshRate())
			SDL_Delay(window.getRefreshRate() - frameTicks);
	}

	window.cleanUp();
	SDL_Quit();

	return 0;
}