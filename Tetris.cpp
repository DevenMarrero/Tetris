#include "Tetris.hpp"

// Tetris
Tetris::Tetris()
{}
Tetris ::~Tetris()
{}

void Tetris::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{

}

void Tetris::handleEvents()
{

}

void Tetris::update()
{

}

void Tetris::clean()
{

}



// Figure
Figure::Figure(int xpos, int ypos)
{
	x = xpos;
	y = ypos;
	// Set random seed
	srand(time(NULL));
	// Pick random figure index
	type = rand() % (figures.size() -1);
	srand(time(NULL));
	colour = rand() % (colours.size() -1);
	rotation = 0;
}

vector<int> Figure::state()
{
	return figures[type][rotation];
}

void Figure::rotate()
{
	rotation = (rotation + 1) % figures[type].size();
}