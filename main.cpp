// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDL.h" // Windows
//#include <SDL2/SDL.h> // Mac

#include <iostream>
#include <time.h> // Random seed
#include <vector>

using namespace std;

class Figure {
public:
    int x;
    int y;

    Figure(int xpos, int ypos) {
        // Set position
        x = xpos;
        y = ypos;
        // Set random seed
        srand(time(NULL));
        // Pick random shape
        type = rand() % (figures.size() - 1);
        srand(time(NULL));
        // Pick random colour
        colour = rand() % (colours.size() - 1);
        rotation = 0;
    }

    // Show figure type and rotation
    vector<int> state() {
        return figures[type][rotation];
    }

    // Rotate figure to next rotation in array
    void rotate() {
        rotation = (rotation + 1) % figures[type].size();
    }

private:
    int type;
    int colour;
    int rotation;

    //Using vectors to easily return and get size of arrays
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


class Tetris {
public:
    Tetris(const char* title, int xpos, int ypos, int width, int height) {
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
                    return;
                }
            }
            else {
                cout << "Error Initializing Window\n";
                return;
            }
        }
        else {
            cout << "Error Initializing SDL\n";
            return;
        }
    }

    // Check user input
    void handleEvents() {
        // Check for some kind of event
        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type) {
            // Exit button pressed
        case SDL_QUIT:
            isRunning = false;
            break;

        default:
            break;
        }
    }

    // Update all objects
    void update() {

    }
    // Update next frame
    void render() {
        SDL_RenderClear(renderer);
        // Render objects here

        // Set background to black and show new frame
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
    }
    // Clear memory after program is complete
    void clean() {
        // Clear memory used by SDL
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        cout << "Game Cleaned\n";
    }

    bool running() {
        return isRunning;
    }

private:
    bool isRunning;
    int score;
    int level;
    Figure figure();
    SDL_Window* window;
    SDL_Renderer* renderer;
};



int main(int argc, char* argv[])
{

    Tetris tetris("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 600);

    while (tetris.running()) {
        tetris.handleEvents();
        tetris.update();
        tetris.render();
    }

    tetris.clean();

    return 0;
}
