#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "RenderWindow.hpp"

int main(int argc, char* args[])
{
	if (SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "SDL_Init has failed." << SDL_GetError() << std::endl;

	if (!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed." << SDL_GetError() << std::endl;

	RenderWindow window("Game v1.0", 1280, 720);

	SDL_Texture* grassTexture = window.loadTexture("ground_grass_1.png");

	bool gameRunning = 1;

	SDL_Event event;

	while (gameRunning)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				gameRunning = 0;
		}

		window.clear();
		window.render(grassTexture);
		window.display();
	}

	window.cleanUp();
	SDL_Quit();

	return 0;
}