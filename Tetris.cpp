#include "Tetris.hpp"

// Tetris Class
Tetris::Tetris()
{}
Tetris ::~Tetris()
{}

void Tetris::init(const char* title, int xpos, int ypos, int width, int height)
{
	isRunning = false;
	// Initialize SDL library
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "SDL Initialized\n";
		// Create the window to draw on
		window = SDL_CreateWindow(title, xpos, ypos, width, height, 0);
		if (window) {
			cout << "Window Created\n";

			// Create the renderer to draw on the window
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer) {
				cout << "Renderer Created\n";
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				isRunning = true;
			}
			else {
				cout << "Error Initializing Renderer\n";
			}
		}
		else {
			cout << "Error Initializing Window\n";
		}
	}
	else{
		cout << "Error Initializing SDL\n";
	}

}

void Tetris::handleEvents()
{
	// Check for some kind of event
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type){
	// Exit button pressed
	case SDL_QUIT:
		isRunning = false;
		break;

	default:
		break;
	}
}

void Tetris::update()
{

}

void Tetris::render()
{
	SDL_RenderClear(renderer);
	// Render objects here

	// Set background to black and show new frame
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}

void Tetris::clean()
{
	// Clear memory used by SDL
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game Cleaned\n";
}