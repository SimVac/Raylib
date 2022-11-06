

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
#include <random>
#include <ctime>

using namespace std;

const int MAX_BALLS = 10;
const int screenWidth = 800;
const int screenHeight = 800;
const double G = 6.67e-11;

struct Vector{
    double x;
    double y;
};

struct Ball{
    double x, y;
    Vector v;
    Color color;
    double mass;
    double r;
};

double map (double n, double start1, double stop1, double start2, double stop2){
    return ((n-start1)/(stop1-start1))*(stop2-start2)+start2;
}

void init_balls (Ball b[]){
    for (int i = 0; i < MAX_BALLS; i++) {
        b[i].r = rand() % 30 + 10;
        b[i].mass = map(b[i].r, 10, 30, 20, 100);
        b[i].x = rand() % (screenWidth - 2 * ((int)b[i].r + 1)) + b[i].r + 1;
        b[i].y = rand() % (screenHeight - 2 * ((int)b[i].r + 1)) + b[i].r + 1;
        b[i].v.x = rand() % 10 - 5;
        b[i].v.y = rand() % 10 - 5;
        b[i].color.r = rand() % 256;
        b[i].color.g = rand() % 256;
        b[i].color.b = rand() % 256;
        b[i].color.a = 255;
    }
//    b[0].r = 30;
//    b[1].r = 30;
//    b[0].x = 100;
//    b[0].y = 100;
//    b[1].x = screenWidth - 100;
//    b[1].y = screenHeight - 100;
//    b[0].color.r = rand() % 256;
//    b[0].color.g = rand() % 256;
//    b[0].color.b = rand() % 256;
//    b[0].color.a = 255;
//    b[1].color.r = rand() % 256;
//    b[1].color.g = rand() % 256;
//    b[1].color.b = rand() % 256;
//    b[1].color.a = 255;
//    b[0].v.x = 0;
//    b[0].v.x = 0;
//    b[1].v.x = 0;
//    b[1].v.x = 0;
//    b[0].mass = map(b[0].r, 10, 30, 20, 100);
//    b[1].mass = map(b[1].r, 10, 30, 20, 100);
}

void draw_balls (Ball b[]){
    for (int i = 0; i < MAX_BALLS; i++) {
        DrawCircle(b[i].x, b[i].y, b[i].r, b[i].color);
    }
}

void collision_balls (Ball &b1, Ball &b2){
    Vector n;
    n.x = b2.x - b1.x;
    n.y = b2.y - b1.y;

    double n_mag = sqrt(n.x * n.x + n.y * n.y);
    Vector un;
    un.x = n.x / n_mag;
    un.y = n.y / n_mag;

    Vector ut;
    ut.x = -un.y;
    ut.y = un.x;

    double v1n, v2n;
    v1n = (un.x * b1.v.x) + (un.y * b1.v.y);
    v2n = (un.x * b2.v.x) + (un.y * b2.v.y);
    double v1t, v2t;
    v1t = (ut.x * b1.v.x) + (ut.y * b1.v.y);
    v2t = (ut.x * b2.v.x) + (ut.y * b2.v.y);

    double v1n_next, v2n_next;
    double den = b1.mass + b2.mass;
    v1n_next = (v1n * (b1.mass - b2.mass) + 2 * b2.mass * v2n) / den;
    v2n_next = (v2n * (b2.mass - b1.mass) + 2 * b1.mass * v1n) / den;

    Vector v1n_next_v, v2n_next_v;
    v1n_next_v.x = un.x * v1n_next;
    v1n_next_v.y = un.y * v1n_next;
    v2n_next_v.x = un.x * v2n_next;
    v2n_next_v.y = un.y * v2n_next;

    Vector v1t_next_v, v2t_next_v;
    v1t_next_v.x = ut.x * v1t;
    v1t_next_v.y = ut.y * v1t;
    v2t_next_v.x = ut.x * v2t;
    v2t_next_v.y = ut.y * v2t;

    Vector v1_next, v2_next;
    v1_next.x = v1n_next_v.x + v1t_next_v.x;
    v1_next.y = v1n_next_v.y + v1t_next_v.y;
    v2_next.x = v2n_next_v.x + v2t_next_v.x;
    v2_next.y = v2n_next_v.y + v2t_next_v.y;

    b1.v.x = v1_next.x;
    b1.v.y = v1_next.y;
    b2.v.x = v2_next.x;
    b2.v.y = v2_next.y;
}

void update_balls (Ball b[]){
    double distance_squared = abs(b[0].x - b[1].x) * abs(b[0].x - b[1].x) + abs(b[0].y - b[1].y) * abs(b[0].y - b[1].y);

    for (int i = 0; i < MAX_BALLS; i++) {
        b[i].x += b[i].v.x;
        b[i].y += b[i].v.y;
//        if (b[i].x + b[i].r > screenWidth || b[i].x - b[i].r < 0)
//            b[i].v.x = -b[i].v.x;
//        if (b[i].y + b[i].r > screenHeight || b[i].y - b[i].r < 0)
//            b[i].v.y = -b[i].v.y;
        if (b[i].x + b[i].r > screenWidth){
            b[i].x = screenWidth - b[i].r;
            b[i].v.x = -b[i].v.x;
        }
        if (b[i].x - b[i].r < 0){
            b[i].x = b[i].r;
            b[i].v.x = -b[i].v.x;
        }
        if (b[i].y + b[i].r > screenHeight){
            b[i].y = screenHeight - b[i].r;
            b[i].v.y = -b[i].v.y;
        }
        if (b[i].y - b[i].r < 0){
            b[i].y = b[i].r;
            b[i].v.y = -b[i].v.y;
        }
    }
    for (int i = 0; i < MAX_BALLS; i++) {
        for (int j = i + 1; j < MAX_BALLS; j++) {
            double d = (b[i].x - b[j].x) * (b[i].x - b[j].x) + (b[i].y - b[j].y) * (b[i].y - b[j].y);
            if (d < (b[i].r + b[j].r) * (b[i].r + b[j].r))
                collision_balls(b[i], b[j]);
        }
    }
}

int main(void)
{
    srand(time(nullptr));
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "Bouncing Balls");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    Ball balls[MAX_BALLS];
    init_balls(balls);
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        update_balls(balls);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground({20, 20, 20, 255});

        draw_balls(balls);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

