#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
	// This will initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL Initialization Failed : " << SDL_GetError() << std::endl;
		return -1;
	}

	// Create Window
	std::cout << "Creating Window...\n";
	SDL_Window* window = SDL_CreateWindow(
		"Ayush's Chess Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1000,
		700,
		SDL_WINDOW_SHOWN
	);
	std::cout << "Window Created Successfully!\n";

	if (window == nullptr)
	{
		std::cout << "Window Creation Failed : " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

	bool running = true;
	SDL_Event event;

	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "Closing Game...\n";
	return 0;
}