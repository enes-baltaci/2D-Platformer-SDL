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

	SDL_Texture* secondPlatform = window.loadTexture("2dplatform.png");

	SDL_Texture* background = window.loadTexture("background.png");

	std::vector<Entity> entitiees = { Entity(Vector2f(0, 0), background, 320, 180),
									  Entity(Vector2f(0, 630), platform, 320, 25),
									  Entity(Vector2f(50, 0), knight, 64, 60),
									  Entity(Vector2f(1280, 630), secondPlatform, 320, 25) };

	bool gameRunning = 1, mouse = 0, down = 0, newPlat = 0;

	SDL_Event event;

	const float timeStep = 0.01f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();

	while (entitiees[2].getMid().y != 550)
	{
		window.clear();

		for (int i = 0; i < 3; i++)
		{
			window.render(entitiees[i]);
		}

		window.display();

		entitiees[2].setPos(entitiees[2].getPos().x, entitiees[2].getPos().y + 1);
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
			if (entitiees[2].getPos().y > 100 && !down)
			{
				entitiees[2].setPos(entitiees[2].getPos().x, entitiees[2].getPos().y - 1);
			}
			else
			{
				entitiees[2].setPos(entitiees[2].getPos().x, entitiees[2].getPos().y + 1);
				down = 1;
			}

			if (entitiees[2].getPos().y == 430)
			{
				mouse = 0;
				down = 0;
			}
		}

		if (newPlat)
		{
			for (Entity& e : entitiees)
			{
				window.render(e);
			}
		}
		else
		{
			for (size_t i = 0; i < 3; i++)
			{
				window.render(entitiees[i]);
			}
		}

		setCursor(0, 0);
		std::cout << utils::hireTimeInSeconds() << std::endl;

		window.display();

		SDL_GetMouseState(&mouseX, &mouseY);
		setCursor(0, 2);
		std::cout << mouseX << ", " << mouseY << std::endl;

		if (entitiees[2].getMid().x < 620 && (entitiees[2].getMid().x - mouseX < 150 && entitiees[2].getMid().x - mouseX > 0 && mouseY - entitiees[2].getPos().y < 200 && mouseY - entitiees[2].getPos().y > 0))
		{
			entitiees[2].setPos(entitiees[2].getPos().x + 1, entitiees[2].getPos().y);
		}
		else if(entitiees[2].getMid().x > 100)
		{
			SDL_Delay(1);
			entitiees[2].setPos(entitiees[2].getPos().x - 1, entitiees[2].getPos().y);
		}

		if (entitiees[1].getPos().x > -1280)
		{
			entitiees[1].setPos(entitiees[1].getPos().x - 1, entitiees[1].getPos().y);
		}

		if (entitiees[3].getPos().x > -1280 && newPlat)
		{
			entitiees[3].setPos(entitiees[3].getPos().x - 1, entitiees[3].getPos().y);
		}
		else newPlat = 0;
		
		if (entitiees[3].getPos().x == -300)
		{
			entitiees[1].setPos(1280, 630);
		}

		if (entitiees[1].getPos().x == -300)
		{
			entitiees[3].setPos(1280, 630);
			newPlat = 1;
		}

		SDL_Delay(1);
	}

	window.cleanUp();
	SDL_Quit();

	return 0;
}