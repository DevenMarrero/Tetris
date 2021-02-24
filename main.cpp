
// Tetris.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SDL.h" // Windows
//#include <SDL2/SDL.h> // Mac

#include <iostream>
#include <time.h> // Random seed
#include <vector>

using namespace std;
const int GRIDSIZE = 30; // size of each square on the grid

class Figure {
public:
    int row;
    int column;

    // Sets figure to random shap and colour
    void reset() {
        // Set position
        row = 19;
        column = 3;
        // Set random seed
        srand(time(NULL));
        // Pick random shape
        type = rand() % (figures.size() - 1);
        srand(time(NULL));
        // Pick random colour
        colour = rand() % (colours.size() - 1);
        rotation = 0;
    }

    // Return figure type and rotation
    vector<int> state() {
        return figures[type][rotation];
    }

    vector<int>getColour() {
        return colours[colour];
    }


    // Rotate figure to next rotation in array
    void rotateRight() {
        rotation = (rotation + 1) % figures[type].size();
    }

    void rotateLeft() {
        rotation--;
        if (rotation < 0) {
            rotation = figures[type].size() - 1;
        }
    }

private:
    int type;
    int rotation;
    int colour;

    /*Using vectors to easily return and get size of arrays
    Vector of all shapes and their rotations in matrix
    * 0  1  2  3
    * 4  5  6  7
    * 8  9  10 11
    * 12 13 14 15
    */
    vector<vector<vector<int>>> figures =
    {
        {{4, 5, 6, 7}, {1, 5, 9, 13}}, // I
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
    int error = 1;


    // Constructor creates screen and sets up game
    Tetris(const char* title, int xpos, int ypos, int width, int height) : shape() {
        // Setup SDL
        // Initialize SDL library
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            cout << "Error Initializing SDL: " << SDL_GetError() << endl;
            return;
        }
        cout << "SDL Initialized\n";
        
        // Create the window to draw on
        SCREEN_WIDTH = width;
        SCREEN_HEIGHT = height;
        window = SDL_CreateWindow(title, xpos, ypos, width, height, 0);
        if (!window) {
            cout << "Error Initializing Window: " << SDL_GetError() << endl;
            return;
        }
        cout << "Window Created\n";

        // Create the renderer to draw on the window
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (!renderer) {
            cout << "Error Initializing Renderer: " << SDL_GetError() << endl;
            return;
        }
        cout << "Renderer Created\n";
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 

        error = 0; // No error
        // Setup Game
        reset();
    }

    // Check user input
    void handleEvents() {
        // Check for some kind of event
        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type) {
        // Exit button pressed
        case SDL_QUIT:
            state = "quit";
            break;

        // Key was pressed
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                moveLeft();
                break;
            case SDLK_RIGHT:
                moveRight();
                break;
            case SDLK_z:
                rotateLeft();
                break;
            case SDLK_UP:
                rotateRight();
                break;
            case SDLK_DOWN:
                break;
            case SDLK_SPACE:
                hardDrop();
                break;
            case SDLK_ESCAPE:
                state = "quit";
                break;

            default:
                break;
            }
            //Start if in menu
            if (state == "start") {
                state = "play";
                break;
            }

        default:
            break;
        }
    }

    // Update all objects
    void update() {

    }

    // Render next frame
    void render() {
        SDL_RenderClear(renderer);
        // Render objects here
        // Menu
        if (state == "start") {

        }

        // game
        else if (state == "play") {
            vector<vector<int>> coords;
            // Temporarily add figure to grid for rendering
            vector<int> colour = shape.getColour();
            for (auto num : shape.state()) {
                int row = shape.row - (num / 4);
                int column = (num - (num / 4) * 4) + shape.column;
                field[row][column][0] = colour[0];
                field[row][column][1] = colour[1];
                field[row][column][2] = colour[2];
                coords.push_back({ row, column });
            }


            for (int row = 0; row < 20; row++) {
                for (int column = 0; column < 10; column++) {
                    SDL_Rect rect;
                    rect.x = column * GRIDSIZE;
                    rect.y = SCREEN_HEIGHT - (row * GRIDSIZE) - GRIDSIZE;
                    rect.w = GRIDSIZE;
                    rect.h = GRIDSIZE;

                    // Colour of square
                    SDL_SetRenderDrawColor(renderer, field[row][column][0], field[row][column][1], field[row][column][2], 255);
                    SDL_RenderFillRect(renderer, &rect);
                    // Border of square
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                    SDL_RenderDrawRect(renderer, &rect);

                }
            }
            // Remove piece from field
            for (auto coord : coords) {
                field[coord[0]][coord[1]] == 0;
            }
            coords.clear();

        }

        // Endscreen
        else if (state == "finish") {

        }

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

    string getState() {
        return state;
    }

private:
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int score;
    int level;
    string state; // start/play/finish/quit
    int field[20][10][3]; // 10x20 playing grid that stores colours


    Figure shape;

    SDL_Window* window;
    SDL_Renderer* renderer;

    // reset game
    void reset() {
        int score = 0;
        int level = 0;
        state = "start";
        new_figure();
        // clear grid
        for (int row = 0; row < 20; row++) {
            for (int column = 0; column < 10; column++) {
                field[row][column][0] = 0; //r
                field[row][column][1] = 0; //g
                field[row][column][2] = 0; //b
            }
        }
    }

    // Get a new figure and start back at the top
    void new_figure() {
        shape.reset();
        if (intersects()) {
            state = "finish";
        }
    }

    void rotateRight() {
        shape.rotateRight();
        if (intersects()) {
            shape.rotateLeft();
        }
    }

    void rotateLeft() {
        shape.rotateLeft();
        if (intersects()) {
            shape.rotateRight();
        }
    }

    void hardDrop() {

    }

    void moveRight() {
        shape.column++;
        if (intersects()) {
            shape.column--;
        }
    }

    void moveLeft() {
        shape.column--;
        if (intersects()) {
            shape.column++;
        }
    }

    bool intersects() {

        if (shape.row < 0) { // Touching bottom
            return true;
        }
        else if (shape.column > 9 || shape.column < 0) { // Touching Sides
            return true;
        }
        //Touching another piece
        vector<int> proximity; // Vector of all nearby pieces
        proximity.clear();
        for (int row = shape.row; row > shape.row - 4; row--) { // Iterate through 4 rows
            if (row < 0) { break; } // Below screen

            for (int column = shape.column; column < shape.column + 4; column++) { // Iterate through 4 columns in row
                if (column > 9) { break; } // Right of screen

                if (field[row][column][0] != 0 || field[row][column][1] != 0 || field[row][column][2] != 0) {
                    // Convert 2d grid to nums in range 0-15
                    proximity.push_back(((shape.row - row) * 4) + (column - shape.column));
                }
            }
        }

        if (proximity.size() > 0) {
            for (auto num : shape.state()) {
                // If position is also in proximity
                if (find(proximity.begin(), proximity.end(), num) == proximity.end()) {
                    return true;
                }
            }
        }

        return false;
    }

    //Lock figure in place on grid
    void freeze() {
        vector<int> colour = shape.getColour();
        for (auto num : shape.state()) {
            int row = shape.row - (num / 4);
            int column = (num - (num / 4) * 4) + shape.column;
            field[row][column][0] = colour[0];
            field[row][column][1] = colour[1];
            field[row][column][2] = colour[2];
        }
    }

};


// MAIN - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int main(int argc, char* argv[])
{
    const int FPS = 60; // How many times screen will refresh per seconds
    const float TICKS_PER_FRAME = 1000 / FPS; // How many milliseconds each frame will take

    Tetris tetris("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 300, 600);
    // Catch problems setting up SLD2
    if (tetris.error != 0) {
        return -1;
    }


    // Gameloop
    while (tetris.getState() != "quit") {
        Uint64 start = SDL_GetPerformanceCounter(); // Frame start time
        
        tetris.handleEvents(); // Get input
        tetris.update(); // Calculate Physics
        tetris.render(); // Draw new frame

        Uint64 end = SDL_GetPerformanceCounter(); // Frame end time
        // Time difference in ms
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        // Wait the remaining time untill the frame is over
        SDL_Delay(floor(TICKS_PER_FRAME - elapsedMS));
    }

    // Clear memory used by SDL2
    tetris.clean();

    return 0;
}
