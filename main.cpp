#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

int main(int argc, char* argv[])
{
	// This will initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL Initialization Failed : " << SDL_GetError() << std::endl;
		return -1;
	}

	// Create Window
	//std::cout << "Creating Window...\n";
	SDL_Window* window = SDL_CreateWindow(
		"Ayush's Chess Engine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		800,
		SDL_WINDOW_SHOWN
	);
	//std::cout << "Window Created Successfully!\n";

	if (window == nullptr)
	{
		std::cout << "Window Creation Failed : " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	// Creating Renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		std::cout << "Renderer could not be created! SDL_error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Code For Square

	SDL_Rect square;

	// Code For Square

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

		// Clear the Screen
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//Draw the chess Board
		
		for (int row = 0; row < 8; row++) 
		{
			for (int column = 0; column < 8; column++) 
			{
				square.x = column * 100;
				square.y = row * 100;
				square.w = 100;
				square.h = 100;

				if ((row + column) % 2 == 0) 
				{
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				}
				else 
				{
					SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
				}
				SDL_RenderFillRect(renderer, &square);
			}

		}

		// For creating texture
		SDL_Surface* surface = IMG_Load("assets/images/white_pawn.png");

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_FreeSurface(surface);
		surface = nullptr;

		SDL_Rect destination;

		destination.x = 0;
		destination.y = 600;
		destination.w = 100;
		destination.h = 100;

		SDL_RenderCopy(renderer, texture, nullptr, &destination);

		// Display Everything
		SDL_RenderPresent(renderer);
	}


	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	std::cout << "Closing Game...\n";
	return 0;
}