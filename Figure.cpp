#include "Figure.hpp"

Figure::Figure(int xpos, int ypos)
{
	// Set position
	x = xpos;
	y = ypos;
	// Set random seed
	srand(time(NULL));
	// Pick random shape
	type = rand() % (figures.size() -1);
	srand(time(NULL));
	// Pick random colour
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