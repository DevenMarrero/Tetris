// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Tetris.hpp"
#include "SDL.h"
using namespace std;

Tetris* tetris = nullptr;

int main(int argc, char *argv[])
{

    tetris = new Tetris();

    tetris->init("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 600);

    while (tetris->running()) {
        tetris->handleEvents();
        tetris->update();
        tetris->render();
    }

    tetris->clean();

    return 0;
}
