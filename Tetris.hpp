#pragma once

#include "SDL.h"
#include <iostream>

using namespace std;

class Tetris
{
public:
	Tetris();
	~Tetris();

	void init(const char* title, int xpos, int ypos, int width, int height);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() { return isRunning; }

private:
	bool isRunning;
	SDL_Window* window;
	SDL_Renderer* renderer;
};

