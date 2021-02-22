#pragma once

#include "SDL.h"
#include "Figure.hpp"
#include <iostream>

using namespace std;

class Tetris
{
public:
	Tetris();
	~Tetris();

	void init(const char* title, int xpos, int ypos, int width, int height);

	// Check user input
	void handleEvents();
	// Update all objects
	void update();
	// Update next frame
	void render();
	// Clear memory after program is complete
	void clean();

	bool running() { return isRunning; }

private:
	bool isRunning;
	int score;
	int level;
	Figure figure();
	SDL_Window* window;
	SDL_Renderer* renderer;
};

