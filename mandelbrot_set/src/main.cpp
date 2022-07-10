

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
#include <cmath>

float map(float value, float start1, float stop1, float start2, float stop2){
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

float abs (float n){
    return n < 0 ? -n : n;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Mandelbrot set");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    const int max_iter = 1000;
    unsigned char brightness[screenWidth][screenHeight];
    for (int x = 0; x < screenWidth; ++x) {
        for (int y = 0; y < screenHeight; ++y) {
            float a = map(x, 0, screenWidth, -2, 2);
            float b = map(y, 0, screenHeight, -2, 2);
            float start_a = a, start_b = b;
            int n;
            for (n = 0; n < max_iter; ++n) {
                float temp_a = a * a - b * b, temp_b = 2 * a * b;
                a = temp_a + start_a;
                b = temp_b + start_b;

                if (abs(a + b) > 1000)
                    break;
            }
            float bright = map(n, 0, max_iter, 0, 1);
            bright = map(std::sqrt(bright), 0, 1, 0, 255);
            if (n == max_iter)
                bright = 0;
            brightness[x][y] = bright;
        }
    }
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        for (int i = 0; i < screenWidth; ++i) {
            for (int j = 0; j < screenHeight; ++j) {
                unsigned char bright = brightness[i][j];
                DrawPixel(i, j, {bright, bright, bright, 255});
            }
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

