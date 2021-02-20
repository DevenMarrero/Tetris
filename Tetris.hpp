#pragma once

#include "SDL.h"
#include <iostream>
#include <time.h>
#include <vector>

using namespace std;

class Tetris
{
public:
	Tetris();
	~Tetris();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

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


class Figure
{
public:
	int x;
	int y;
	
	// Constructor
	Figure(int x, int y);

	// Show figure type and rotation
	vector<int> state();

	// Rotate figure to next rotation in array
	void rotate();

private:
	int type;
	int colour;
	int rotation;
	/* Vector of all shapes and their rotations in matrix
	* 0  1  2  3
	* 4  5  6  7
	* 8  9  10 11
	* 12 13 14 15
	*/
	vector<vector<vector<int>>> figures =
	{
		{{1, 5, 9, 13}, {4, 5, 6, 7}}, // I
		{{4, 5, 9, 10}, {2, 6, 5, 9}}, // Z
		{{6, 7, 9, 10}, {1, 5, 6, 10}}, // S
		{{1, 2, 5, 9}, {0, 4, 5, 6}, {1, 5, 9, 8}, {4, 5, 6, 10}}, // J
		{{1, 2, 6, 10}, {5, 6, 7, 9}, {1, 6, 10, 11}, {3, 5, 6, 7}}, // L
		{{1, 4, 5, 6}, {1, 4, 5, 9}, {4, 5, 6, 9}, {1, 5, 6, 9}}, // T
		{{1, 2, 5, 6}} // O
	};

	vector<vector<int>> colours =
	{
		{120, 37, 179},
		{100, 179, 179},
		{80, 34, 22},
		{80, 134, 22},
		{180, 34, 22},
		{180, 34, 122},
	};

};
