#include <iostream>
#include <conio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <Windows.h>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Utils.hpp"

#define BACKGROUND_ENTITY_ID (0)
#define PLATFORM1_ENTITY_ID (1)
#define PLAYER_ENTITY_ID (2)
#define PLATFORM2_ENTITY_ID (3)
#define GAMEOVER (4)
#define GROUNDED (630)

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

	SDL_Texture* endGame = window.loadTexture("game_over.png");

	std::vector<Entity> entitiees = { Entity(Vector2f(0, 0), background, 320, 180),
									  Entity(Vector2f(0, 630), platform, 320, 25),
									  Entity(Vector2f(50, 0), knight, 64, 60),
									  Entity(Vector2f(1280, 630), secondPlatform, 320, 25),
									  Entity(Vector2f(250, 70), endGame, 200, 100) };

	bool gameRunning = 1, mouse = 0, down = 0, newPlat = 0, gameOver = 0;

	SDL_Event event;

	const float timeStep = 0.01f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();

	while (entitiees[PLAYER_ENTITY_ID].getMid().y != 550)
	{
		window.clear();

		for (int i = 0; i < 3; i++)
		{
			window.render(entitiees[i]);
		}

		window.display();

		entitiees[PLAYER_ENTITY_ID].setPos(entitiees[PLAYER_ENTITY_ID].getPos().x, entitiees[PLAYER_ENTITY_ID].getPos().y + 1);
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

		if (!gameOver && mouse)
		{
			if (entitiees[PLAYER_ENTITY_ID].getPos().y > 100 && !down)
			{
				entitiees[PLAYER_ENTITY_ID].setPos(entitiees[PLAYER_ENTITY_ID].getPos().x, entitiees[PLAYER_ENTITY_ID].getPos().y - 1);
			}
			else
			{
				entitiees[PLAYER_ENTITY_ID].setPos(entitiees[PLAYER_ENTITY_ID].getPos().x, entitiees[PLAYER_ENTITY_ID].getPos().y + 1);
				down = 1;
			}

			if (entitiees[PLAYER_ENTITY_ID].getPos().y == 430)
			{
				mouse = 0;
				down = 0;
			}
		}

		if (newPlat)
		{
			for (size_t i = 0; i < 4; i++)
			{
				window.render(entitiees[i]);
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

		if (!gameOver)
		{
			if (entitiees[PLAYER_ENTITY_ID].getMid().x < 620 && (entitiees[PLAYER_ENTITY_ID].getMid().x - mouseX < 150 && entitiees[PLAYER_ENTITY_ID].getMid().x - mouseX > 0 && mouseY - entitiees[PLAYER_ENTITY_ID].getPos().y < 200 && mouseY - entitiees[PLAYER_ENTITY_ID].getPos().y > 0))
			{
				entitiees[PLAYER_ENTITY_ID].setPos(entitiees[PLAYER_ENTITY_ID].getPos().x + 1, entitiees[PLAYER_ENTITY_ID].getPos().y);
			}
			else if (entitiees[PLAYER_ENTITY_ID].getMid().x > 100)
			{
				SDL_Delay(1);
				entitiees[PLAYER_ENTITY_ID].setPos(entitiees[PLAYER_ENTITY_ID].getPos().x - 1, entitiees[PLAYER_ENTITY_ID].getPos().y);
			}
		}
		else
		{
			if (entitiees[PLAYER_ENTITY_ID].getPos().y < 720)
			{
				entitiees[PLAYER_ENTITY_ID].setPos(entitiees[PLAYER_ENTITY_ID].getPos().x, entitiees[PLAYER_ENTITY_ID].getPos().y + 1);
			}
			else if (entitiees[PLAYER_ENTITY_ID].getPos().y == 720)
			{
				gameRunning = 0;

				window.render(entitiees[GAMEOVER]);

				window.display();
			}
		}

		if (entitiees[PLATFORM1_ENTITY_ID].getPos().x > -1280)
		{
			entitiees[PLATFORM1_ENTITY_ID].setPos(entitiees[PLATFORM1_ENTITY_ID].getPos().x - 1, entitiees[PLATFORM1_ENTITY_ID].getPos().y);
		}

		if (entitiees[PLATFORM2_ENTITY_ID].getPos().x > -1280 && newPlat)
		{
			entitiees[PLATFORM2_ENTITY_ID].setPos(entitiees[PLATFORM2_ENTITY_ID].getPos().x - 1, entitiees[PLATFORM2_ENTITY_ID].getPos().y);
		}
		else newPlat = 0;

		if (entitiees[PLATFORM2_ENTITY_ID].getPos().x == -300)
		{
			entitiees[PLATFORM1_ENTITY_ID].setPos(1280, 630);
		}

		if (entitiees[PLATFORM1_ENTITY_ID].getPos().x == -300)
		{
			entitiees[PLATFORM2_ENTITY_ID].setPos(1280, 630);
			newPlat = 1;
		}

		if (entitiees[PLAYER_ENTITY_ID].getFeetY() == GROUNDED)
		{
			if (((entitiees[PLAYER_ENTITY_ID].getFeetX().x > entitiees[PLATFORM1_ENTITY_ID].getPos().x && entitiees[PLAYER_ENTITY_ID].getFeetX().y > entitiees[PLATFORM1_ENTITY_ID].getPos().x + 1280) &&
				(entitiees[PLAYER_ENTITY_ID].getFeetX().x < entitiees[PLATFORM2_ENTITY_ID].getPos().x && entitiees[PLAYER_ENTITY_ID].getFeetX().y < entitiees[PLATFORM2_ENTITY_ID].getPos().x + 1280)) ||
				((entitiees[PLAYER_ENTITY_ID].getFeetX().x > entitiees[PLATFORM2_ENTITY_ID].getPos().x && entitiees[PLAYER_ENTITY_ID].getFeetX().y > entitiees[PLATFORM2_ENTITY_ID].getPos().x + 1280) &&
				(entitiees[PLAYER_ENTITY_ID].getFeetX().x < entitiees[PLATFORM1_ENTITY_ID].getPos().x && entitiees[PLAYER_ENTITY_ID].getFeetX().y < entitiees[PLATFORM1_ENTITY_ID].getPos().x + 1280)))
			{
				gameOver = 1;
			}
		}

		SDL_Delay(1);
	}
	std::cout << "Press a key to exit.";
	_getch();

	window.cleanUp();
	SDL_Quit();

	return 0;
}