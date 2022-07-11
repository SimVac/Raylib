

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

    void show(int dim, int screenWidth, int screenHeight) const {
        DrawRectangle(pos.x, pos.y, screenWidth / dim, screenHeight / dim, WHITE);
        if (!active)
            DrawRectangle(pos.x + 1, pos.y + 1, screenWidth / dim - 2, screenHeight / dim - 2, BLACK);
    }

    void click(){
        active = true;
    }
};

void detect_click (std::vector<Tile> &tiles, int screenWidth, int screenHeight, int dim){
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return;
    Vector2D mouse;
    mouse.x = GetMouseX();
    mouse.y = GetMouseY();
    int j = mouse.x / (screenWidth / dim), i = mouse.y / (screenHeight / dim);
    tiles.at(j + i * dim).click();
}

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
    std::vector<Tile> tiles;
    for (int i = 0; i < dim * dim; ++i) {
        Tile temp = Tile((i % dim) * screenWidth / dim, (i / dim) * screenHeight / dim);
        tiles.push_back(temp);
    }

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        detect_click(tiles, screenWidth, screenHeight, dim);
        BeginDrawing();

        ClearBackground(BLACK);

        for (const auto &t: tiles) {
            t.show(dim, screenWidth, screenHeight);
        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

