#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Utils.hpp"

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "SDL_Init has failed." << SDL_GetError() << std::endl;

	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed." << SDL_GetError() << std::endl;

	RenderWindow window("Game v1.0", 1280, 720);

	std::cout << window.getRefreshRate() << std::endl;

	SDL_Texture* grassTexture = window.loadTexture("ground_grass_1.png");

	std::vector<Entity> entitiees = { Entity(Vector2f(150, 150), grassTexture) };

	bool gameRunning = 1;

	SDL_Event event;

	const float timeStep = 0.01f;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds();

	while (gameRunning)
	{
		int startTicks = SDL_GetTicks();

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
			}

			accumulator -= timeStep;
		}

		const float alpha = accumulator / timeStep;

		window.clear();

		for (Entity& e : entitiees)
		{
			window.render(e);
		}

		//std::cout << utils::hireTimeInSeconds() << std::endl;

		window.display();

		int mouseX, mouseY;

		SDL_GetMouseState(&mouseX, &mouseY);

		if (mouseX - entitiees[0].getMid().x < 150 && mouseX - entitiees[0].getMid().x > 0 && mouseY - entitiees[0].getPos().y < 130 && mouseY - entitiees[0].getPos().y > 0)
		{
			entitiees[0].setPos(entitiees[0].getPos().x - 3 , entitiees[0].getPos().y);
		}
		if (entitiees[0].getMid().x - mouseX < 150 && entitiees[0].getMid().x - mouseX > 0 && mouseY - entitiees[0].getPos().y < 130 && mouseY - entitiees[0].getPos().y > 0)
		{
			entitiees[0].setPos(entitiees[0].getPos().x + 3, entitiees[0].getPos().y);
		}
		if (mouseY - entitiees[0].getMid().y < 150 && mouseY - entitiees[0].getMid().y > 0 && mouseX - entitiees[0].getPos().x > 0 && mouseX - entitiees[0].getPos().x < 130)
		{
			entitiees[0].setPos(entitiees[0].getPos().x, entitiees[0].getPos().y - 3);
		}
		if (entitiees[0].getMid().y - mouseY < 150 && entitiees[0].getMid().y - mouseY > 0 && mouseX - entitiees[0].getPos().x > 0 && mouseX - entitiees[0].getPos().x < 130)
		{
			entitiees[0].setPos(entitiees[0].getPos().x, entitiees[0].getPos().y + 3);
		}

		std::cout << mouseX << ", " << mouseY << std::endl;

		int frameTicks = SDL_GetTicks() - startTicks;

		if (frameTicks < 1000 / window.getRefreshRate())
			SDL_Delay(window.getRefreshRate() - frameTicks);
	}

	window.cleanUp();
	SDL_Quit();

	return 0;
}