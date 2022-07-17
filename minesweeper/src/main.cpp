

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
    int near_bombs;
    bool flag;

    bool valid(int i, int j, int dim){
        if (i < 0 || i >= dim || j < 0 || j >= dim)
            return false;
        return true;
    }

    int count_near_bombs(const std::vector<Tile> &tiles, int dim, int pos_in_vector){
        int count = 0;
        int pos_in_vec_i = pos_in_vector / dim , pos_in_vec_j = pos_in_vector % dim;
        for (int i = -1; i < 2; ++i) {
            for (int j = -1; j < 2; ++j) {
                if (i == 0 && j == 0)
                    continue;
                if (valid(pos_in_vec_j + j, pos_in_vec_i + i, dim)){
                    if (tiles.at(pos_in_vec_j + j + (pos_in_vec_i + i) * dim).isBomb())
                        count++;
                }
            }
        }
        return count;
    }
public:

    Tile(int x, int y){
        pos.x = x;
        pos.y = y;
        active = false;
        bomb = false;
        flag = false;
    }

    void setBomb (bool val){
        bomb = val;
    }

    bool isBomb () const{
        return bomb;
    }

    void setFlag (bool val){
        flag = val;
    }

    bool getFlag () {
        return flag;
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

    int getNearbombs () const{
        return near_bombs;
    }

    void setNearbombs (std::vector<Tile> &tiles, int dim, int pos_in_vector){
        near_bombs = count_near_bombs(tiles, dim, pos_in_vector);
    }
};

class Grid{
    std::vector<Tile> tiles;
    int dim, screenWidth, screenHeight;

    bool valid (int x, int y) const{
        if (x < 0 || x >= dim || y < 0 || y >= dim)
            return false;
        return true;
    }

    void generate_bombs (int n){
        std::vector<int> numbers;
        for (int i = 0; i < tiles.size(); ++i) {
            numbers.push_back(i);
        }
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));
        for (int i = 0; i < n; ++i) {
            tiles.at(numbers.at(i)).setBomb(true);
        }
    }

public:

    Grid(int dim, int screenWidth, int screenHeight){
        for (int i = 0; i < dim * dim; ++i) {
            Tile temp = Tile((i % dim) * screenWidth / dim, (i / dim) * screenHeight / dim);
            tiles.push_back(temp);
        }
        generate_bombs(10);
        for (int i = 0; i < dim * dim; ++i) {
            tiles.at(i).setNearbombs(tiles, dim, i);
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
            if (tile.isActive()) {
                DrawRectangle(tile.getPos().x, tile.getPos().y, d - 1, d - 1, WHITE);
                if (tile.isBomb())
                    DrawText("B", tile.getPos().x + 27, tile.getPos().y + d - 60, 50, BLACK);
                else
                    DrawText(std::to_string(tile.getNearbombs()).c_str(), tile.getPos().x + 27, tile.getPos().y + d - 60, 50, BLACK);
            }else if (tile.getFlag()){
                DrawText("F", tile.getPos().x + 27, tile.getPos().y + d - 60, 50, WHITE);
            }
        }
    }

    void expand_click (int x, int y){
        for (int i = -1; i < 2; ++i) {
            for (int j = -1; j < 2; ++j) {
                if (valid(x + j, y + i)){
                    if (!tiles.at((x + j) + (y + i) * dim).isActive()){
                        tiles.at((x + j) + (y + i) * dim).click();
                        if (tiles.at((x + j) + (y + i) * dim).getNearbombs() == 0)
                            expand_click(x + j, y + i);
                    }
                }
            }
        }
    }

    void detect_click (){
        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            return;
        Vector2D mouse;
        mouse.x = GetMouseX();
        mouse.y = GetMouseY();
        int j = mouse.x / (screenWidth / dim), i = mouse.y / (screenHeight / dim);
        int pos = j + i * dim;
        if (tiles.at(pos).getFlag())
            return;
        tiles.at(pos).click();
        if (tiles.at(pos).getNearbombs() == 0 && !tiles.at(pos).isBomb())
            expand_click(j, i);
    }

    void detect_flag (){
        if (!IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
            return;
        Vector2D mouse;
        mouse.x = GetMouseX();
        mouse.y = GetMouseY();
        int j = mouse.x / (screenWidth / dim), i = mouse.y / (screenHeight / dim);
        int pos = j + i * dim;
        if (tiles.at(pos).isActive())
            return;
        tiles.at(pos).setFlag(!tiles.at(pos).getFlag());
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
        grid.detect_flag();

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

