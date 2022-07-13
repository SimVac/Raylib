

/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "bits/stdc++.h"

struct Vector2D{
    int x, y;
};

class Tile{
    Vector2D pos;
    bool bomb;
    bool active;
public:

    Tile(int x, int y){
        pos.x = x;
        pos.y = y;
        active = false;
        bomb = false;
    }

    void setBomb (bool val){
        bomb = val;
    }

    bool isActive() const{
        return active;
    }

    Vector2D getPos() const{
        return pos;
    }

    void click(){
        active = true;
    }
};

class Grid{
    std::vector<Tile> tiles;
    int dim, screenWidth, screenHeight;
public:
    Grid(int dim, int screenWidth, int screenHeight){
        for (int i = 0; i < dim * dim; ++i) {
            Tile temp = Tile((i % dim) * screenWidth / dim, (i / dim) * screenHeight / dim);
            tiles.push_back(temp);
        }
        this->dim = dim;
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
    }

    void show () const{
        int d = screenWidth / dim;
        for (int i = 0; i < dim; ++i) {
            DrawLine(i * d, 0, i * d, screenHeight, GRAY);
            DrawLine(0, i * d, screenWidth, i * d, GRAY);
        }
        for (auto tile : tiles) {
            if (tile.isActive())
                DrawRectangle(tile.getPos().x, tile.getPos().y, d - 1, d - 1, WHITE);
        }
    }

    void detect_click (){
        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            return;
        Vector2D mouse;
        mouse.x = GetMouseX();
        mouse.y = GetMouseY();
        int j = mouse.x / (screenWidth / dim), i = mouse.y / (screenHeight / dim);
        tiles.at(j + i * dim).click();
    }
};


int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Minesweeper");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    int dim = 10;
    Grid grid = Grid(dim, screenWidth, screenHeight);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        grid.detect_click();

        BeginDrawing();

        ClearBackground(BLACK);
        grid.show();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

