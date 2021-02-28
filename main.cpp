//  main.cpp
//
//  Title: Tetris
//  Description: 2d Tetris clone made with SDL2
//  Created by: Deven
//  Created on: Feb 17th, 2021
//  Last Updated: Feb 24th, 2021
//  Known Limitations:

#include "SDL.h" // Windows
#include "SDL_ttf.h" // Windows

//#include <SDL2/SDL.h> // Mac
//#include <SDL2_ttf/SDL_ttf.h> // Mac

#include <iostream>
#include <time.h> // Random seed
#include <vector>
#include <sstream>

using namespace std;

class Figure {
public:
    int row;
    int column;

    // Sets figure to random shap and colour
    void reset() {
        // Set position
        row = 19;
        column = 3;
        // Pick random shape
        type = rand() % figures.size();
        // Pick Colour
        colour = type;
        rotation = 0;
    }

    // Return figure type and rotation
    vector<int> state() {
        return figures[type][rotation];
    }

    // Return rgb values of figure
    vector<int>getColour() {
        return colours[colour];
    }


    // Rotate figure to previous rotation in array
    void rotateRight() {
        rotation--;
        if (rotation < 0) {
            rotation = figures[type].size() - 1;
        }
    }

    // Rotate figure to next rotation in array
    void rotateLeft() {
        rotation = (rotation + 1) % figures[type].size();
    }
    
    void resetRotation(){
        rotation = 0;
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
        {{1, 2, 6, 10}, {5, 6, 7, 9}, {1, 5, 9, 10}, {3, 5, 6, 7}}, // L
        {{1, 4, 5, 6}, {1, 4, 5, 9}, {4, 5, 6, 9}, {1, 5, 6, 9}}, // T
        {{1, 2, 5, 6}} // O
    };

    vector<vector<int>> colours =


    {
        {33, 235, 225}, // Cyan
        {235, 33, 33}, // Red
        {27, 196, 47}, // Green
        {30, 67, 214}, // blue
        {224, 139, 34}, // Orange
        {164, 34, 224}, //Purple
        {242, 239, 24} //Yellow
    };

};


class Tetris {
public:
    int error = 1;


    // Constructor creates screen and sets up game
    Tetris(const char* title, int xpos, int ypos, int width, int height, int gridSize) : shape() {
        // Setup SDL
        // Initialize SDL2 library
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            cout << "Error Initializing SDL: " << SDL_GetError() << endl;
            return;
        }
        cout << "SDL Initialized\n";

        // Initialize fonts library
        if (TTF_Init() == -1) {
            cout << "Error Initializing SDL_TTF\n";
            return;
        }
        cout << "SDL_TTF Initialized\n";
        infoFont = TTF_OpenFont("/Library/Fonts/Arial.ttf", 30);
        if (!infoFont) {
            infoFont = TTF_OpenFont("arial.ttf", 30);
            if (!infoFont) {
                cout << "Error opening font";
            }
        }
        
        // Create the window to draw on
        GRIDSIZE = gridSize;
        SCREEN_WIDTH = width;
        SCREEN_HEIGHT = height;
        GRID_WIDTH = GRIDSIZE * 10;
        GRID_HEIGHT = GRIDSIZE * 20;
        OFFSET = (width / 2) - (GRIDSIZE * 5); // Xpos of grid start
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
            if (state == "start") {
                state = "play";
                if (event.key.keysym.sym == SDLK_ESCAPE){
                    state = "quit";
                }
                break;
            }
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
                softDrop();
                break;
            case SDLK_SPACE:
                hardDrop();
                break;
            case SDLK_c:
                hold();
                break;

            case SDLK_ESCAPE:
                state = "quit";
                break;

            default:
                break;
            }

        default:
            break;
        }
    }

    // Update figure object
    void update() {
        if (state == "play"){
            level = (linesCleared / 10) + 1;
            float frameDiff = 16.6666667; // Original NES time(MS) between frames
            float delay; // MS between each drop (based on level)
            
            if (level >=0 && level <= 8){
                delay = 800.0f - (5.f * level * frameDiff); // Ranged between 48-8 frames
            }
            else if (level == 9){
                delay = frameDiff * 6;
            }
            else if (level >= 10 && level <= 12){
                delay = frameDiff * 5;
            }
            else if (level >= 13 && level <= 15){
                delay = frameDiff * 4;
            }
            else if (level >= 16 && level <= 18){
                delay = frameDiff * 3;
            }
            else if (level >= 19 && level <= 28){
                delay = frameDiff * 2;
            }
            else {
                delay = frameDiff;
            }
            
            Uint32 endMS = SDL_GetTicks();
            float elapsedMS = endMS - startMS;
            // Time difference in ms
            
            if (elapsedMS >= delay){
                // Move down
                shape.row--;
                // Hit something
                if (intersects()){
                    shape.row++;
                    freeze();
                    breakLines();
                }
                startMS = SDL_GetTicks();
            }
        }
        
    }

    // Render next frame
    void render() {
        // Set background to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Color white = { 255, 255, 255, 255 };
        
        // Render objects here
        // Menu
        if (state == "start") {

        }

        // game
        else if (state == "play") {
            // Grid
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
            
            // Draw grid
            for (int row = 0; row < 20; row++) {
                for (int column = 0; column < 10; column++) {
                    SDL_Rect rect;
                    rect.x = column * GRIDSIZE + OFFSET;
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
                field[coord[0]][coord[1]][0] = 0;
                field[coord[0]][coord[1]][1] = 0;
                field[coord[0]][coord[1]][2] = 0;
            }
            coords.clear();
            

            int gridTop = SCREEN_HEIGHT - GRID_HEIGHT;
            
            // Score/level/lines - - -
            // Set dimensions of border
            SDL_Rect infoBorder;
            infoBorder.x = OFFSET - 200;
            infoBorder.y = SCREEN_HEIGHT - GRID_HEIGHT / 3  - GRID_HEIGHT / 7;
            infoBorder.w = 175;
            infoBorder.h = SCREEN_HEIGHT - infoBorder.y;
            
            // Positions inside Border
            int xCenter = infoBorder.x + infoBorder.w / 2;
            int yTop = infoBorder.y + 25;
            int yCenter = SCREEN_HEIGHT - infoBorder.h / 2 - 25;
            int yBottom = SCREEN_HEIGHT - 75;
            
            // Draw Border
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &infoBorder);
            
            // Draw info
            renderText(xCenter, yTop, "SCORE", infoFont, white);
            renderText(xCenter, yTop + 35, to_string(score), infoFont, white);
            
            renderText(xCenter, yCenter, "LEVEL", infoFont, white);
            renderText(xCenter, yCenter + 35, to_string((int)level), infoFont, white);
            
            renderText(xCenter, yBottom, "LINES", infoFont, white);
            renderText(xCenter, yBottom + 35, to_string(linesCleared), infoFont, white);


            // Next pieces - - -
            // Set dimensions of border
            SDL_Rect nextBorder;
            nextBorder.x = OFFSET + GRID_WIDTH + 25; // Right of grid
            nextBorder.y = gridTop + 50;
            nextBorder.w = 4 * (GRIDSIZE + 5) + 30; // Width of nextField grid
            nextBorder.h = 14 * (GRIDSIZE + 5); // Height of nextField grid

            // Draw text
            renderText(nextBorder.x + nextBorder.w /2, gridTop + 25, "NEXT", infoFont, white);

            // Draw border
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &nextBorder);


            // Clear grid
            int nextField[14][4][3];

            for (int row = 0; row < 14; row++) {
                for (int column = 0; column < 4; column++) {
                    nextField[row][column][0] = 0;
                    nextField[row][column][1] = 0;
                    nextField[row][column][2] = 0;
                }
            }

            // Add pieces to grid
            int space = 0;
            for (auto shape : nextShapes) {
                colour = shape.getColour();
                for (auto num : shape.state()) {

                    int row = (num / 4) + space;
                    int column = num - (num / 4) * 4;
                    nextField[row][column][0] = colour[0];
                    nextField[row][column][1] = colour[1];
                    nextField[row][column][2] = colour[2];
                }
                space+= 5;
            }

            // Draw grid
            for (int row = 0; row < 14; row++) {
                for (int column = 0; column < 4; column++) {
                    SDL_Rect nextRect;
                    nextRect.x = column * (GRIDSIZE + 5) + nextBorder.x + 15;
                    nextRect.y = nextBorder.y + (row * GRIDSIZE) + 15;
                    nextRect.w = GRIDSIZE + 5; // Larger square
                    nextRect.h = GRIDSIZE + 5;

                    // Colour of square
                    SDL_SetRenderDrawColor(renderer, nextField[row][column][0], nextField[row][column][1], nextField[row][column][2], 255);
                    SDL_RenderFillRect(renderer, &nextRect);
                    
                    // Border of square of it has a piece
                    if (nextField[row][column][0] != 0 || nextField[row][column][1] != 0 || nextField[row][column][2] != 0) {
                        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                        SDL_RenderDrawRect(renderer, &nextRect);
                    }

                }
            }


            // Hold - - -
            SDL_Rect holdBorder;
            holdBorder.x = OFFSET - 200;
            holdBorder.y = gridTop + 50;
            holdBorder.w = 175;
            holdBorder.h = 175;

            // Draw text
            renderText(holdBorder.x + holdBorder.w / 2, gridTop + 25, "HOLD", infoFont, white);

            // Draw border
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &holdBorder);
            
            // Clear piece grid
            int holdField[4][4][3] = 
            {
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
                {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
            };
            
            // Add piece to grid
            if (holdShape.row != -1){
                colour = holdShape.getColour();
                for (auto num : holdShape.state()){
                    int row = (num / 4);
                    int column = num - (num / 4) * 4;
                    holdField[row][column][0] = colour[0];
                    holdField[row][column][1] = colour[1];
                    holdField[row][column][2] = colour[2];
                }
            }
            
            // Draw grid
            for (int row = 3; row > -1; row--){
                for (int column = 0; column < 4; column++){
                    SDL_Rect holdRect;
                    holdRect.x = column * (GRIDSIZE + 5) + holdBorder.x + 15;
                    holdRect.y = (holdBorder.y + holdBorder.h - 15) - (row * (GRIDSIZE + 5)) - (GRIDSIZE + 5);
                    holdRect.w = GRIDSIZE + 5;  // Larger square
                    holdRect.h = GRIDSIZE + 5;

                    // Colour of square
                    SDL_SetRenderDrawColor(renderer, holdField[row][column][0], holdField[row][column][1], holdField[row][column][2], 255);
                    SDL_RenderFillRect(renderer, &holdRect);
                    
                    // Border of square of it has a piece
                    if (holdField[row][column][0] != 0 || holdField[row][column][1] != 0 || holdField[row][column][2] != 0){
                        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                        SDL_RenderDrawRect(renderer, &holdRect);
                    }
                }
            }
            
        }

        // Endscreen
        else if (state == "finish") {

        }

        // Show new frame
        SDL_RenderPresent(renderer);
    }

    // Clear memory after program is complete
    void clean() {
        // Clear memory used by SDL
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        TTF_Quit();
        cout << "Game Cleaned\n";
    }

    string getState() {
        return state;
    }

private:
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int GRIDSIZE;
    int OFFSET;
    int GRID_HEIGHT;
    int GRID_WIDTH;
    int score;
    float level;
    int linesCleared;
    int swapped;
    bool held;
    
    TTF_Font* infoFont;
    
    Uint32 startMS;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    Figure shape;
    Figure holdShape;
    vector<Figure> nextShapes;
    
    string state; // start/play/finish/quit
    int field[20][10][3]; // 10x20 playing grid that stores colours

    // reset game
    void reset() {
        score = 0;
        level = 1;
        linesCleared = 0;
        state = "start";

        // Reset current shape
        shape.reset();

        // Used to mark empty holdshape
        holdShape.row = -1;
        // Get next 3 shapes
        for (int i = 0; i < 3; i++) {
            Figure nextShape;
            nextShape.reset();
            nextShapes.push_back(nextShape);
        }

        // clear grid
        for (int row = 0; row < 20; row++) {
            for (int column = 0; column < 10; column++) {
                field[row][column][0] = 0; //r
                field[row][column][1] = 0; //g
                field[row][column][2] = 0; //b
            }
        }
        startMS = SDL_GetTicks(); // Frame start time
    }

    // Get a new figure and start back at the top
    void new_figure() {
        shape = nextShapes[0];
        nextShapes[0] = nextShapes[1];
        nextShapes[1] = nextShapes[2];
        nextShapes[2].reset();
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
        int distance = 0;
        while (!intersects()) {
            shape.row--;
            distance++;
        }
        shape.row++;
        distance--;
        score += distance * 2;
        freeze();
        breakLines();
    }
    
    void softDrop(){
        shape.row--;
        score++;
        if (intersects()){
            shape.row++;
            score--;
        }

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

    void hold() {
        if (!held){
            if (holdShape.row != -1) {
                holdShape.row = 19;
                holdShape.column = 3;
                shape.resetRotation();
                swap(holdShape, shape);
            }
            else {
                holdShape = shape;
                new_figure();
            }
            held = true;
        }
    }

    // Check if piece is touching anything
    bool intersects() {

        vector<int> proximity; // Vector of all nearby pieces
        proximity.clear();
        // Iterate through 4 rows
        for (int row = shape.row; row > shape.row - 4; row--) {
            // Iterate through 4 columns in row
            for (int column = shape.column; column < shape.column + 4; column++) {

                // If touching other piece
                if (field[row][column][0] != 0 || field[row][column][1] != 0 || field[row][column][2] != 0) {
                    // Convert 2d grid to nums in range 0-15
                    int val = ((shape.row - row) * 4) + (column - shape.column);
                    proximity.push_back(val); // Add as possibility
                }
                // Touching edge of screen
                else if (row < 0 || column > 9 || column < 0) {
                    int val = ((shape.row - row) * 4) + (column - shape.column);
                    proximity.push_back(val); // Add as possibility
                }
            }
        }
        
        // Check possible collisions for match
        if (proximity.size() > 0) {
            for (auto num : shape.state()) {
                // If position is also in proximity they are colliding
                if (find(proximity.begin(), proximity.end(), num) != proximity.end()) {
                    return true;
                }
            }
        }
        return false;
    }
    
    void breakLines() {
        int linesBroken = 0;
        for (int row = 0; row < 20; row++) {
            bool full = true;
            for (int column = 0; column < 10; column++) {
                // If empty square check next row
                if (field[row][column][0] == 0 && field[row][column][1] == 0 && field[row][column][2] == 0) {
                    full = false;
                    break;
                }
            }
            // If row is full move everything above down
            if (full) {
                linesBroken++;
                linesCleared++;
                for (int rRow = row + 1; rRow < 20; rRow++) {
                    for (int rColumn = 0; rColumn < 10; rColumn++) {
                        field[rRow - 1][rColumn][0] = field[rRow][rColumn][0];
                        field[rRow - 1][rColumn][1] = field[rRow][rColumn][1];
                        field[rRow - 1][rColumn][2] = field[rRow][rColumn][2];
                    }
                }
                row--;
            }

        }
        
        // Add score
        if (linesBroken == 1) {
            score += 100 * level;
        }
        else if (linesBroken == 2) {
            score += 300 * level;
        }
        else if (linesBroken == 3) {
            score += 500 * level;
        }
        else if (linesBroken == 4) {
            score += 800 * level;
        }
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
        // Create a new figure
        new_figure();
        // Let user hold again
        held = false;
    }

    // For displaying text on screen
    void renderText(int xpos, int ypos, string text, TTF_Font* font, SDL_Color& colour) {
        SDL_Rect position;
        position.x = xpos;
        position.y = ypos;

        // Create texture
        SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), colour);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        // Set width and height
        SDL_QueryTexture(texture, nullptr, nullptr, &position.w, &position.h);
        
        position.x -= position.w / 2;
        position.y -= position.h /2;

        // Draw texture
        SDL_RenderCopy(renderer, texture, nullptr, &position);
    }


};


// MAIN - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int main(int argc, char* argv[])
{
    const int FPS = 30; // How many times screen will refresh per seconds
    const float TICKS_PER_FRAME = 1000 / FPS; // Calculate how many milliseconds each frame will take
    // Set random seed for pieces
    srand(time(NULL));

    Tetris tetris("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 30); // 300 600
    // Catch problems setting up SLD2
    if (tetris.error != 0) {
        return -1;
    }

    // Gameloop
    while (tetris.getState() != "quit") {
        Uint64 start = SDL_GetPerformanceCounter(); // Frame start time
        
        // Frame
        tetris.handleEvents(); // Get input
        tetris.update(); // Calculate Physics
        tetris.render(); // Draw new frame

        Uint64 end = SDL_GetPerformanceCounter(); // Frame end time
        // Time difference in ms
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        // Wait the remaining time untill the frame is over is there is time
        float delay = floor(TICKS_PER_FRAME - elapsedMS);
        if (delay > 0){
            SDL_Delay(delay);
        }
    }

    // Clear memory used by SDL2
    tetris.clean();

    return 0;
}
