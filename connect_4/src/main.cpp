

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
#include <string>

const int BOARD_HEIGHT = 6;
const int BOARD_WIDTH = 7;
const int screenWidth = 1000;
const int screenHeight = 800;

void drawBoard (int board[][BOARD_WIDTH]){
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (board[i][j] == 1)
                DrawRectangle(j * screenWidth / BOARD_WIDTH, i * screenHeight / BOARD_HEIGHT, screenWidth / BOARD_WIDTH, screenHeight / BOARD_HEIGHT, YELLOW);
            else if (board[i][j] == 2)
                DrawRectangle(j * screenWidth / BOARD_WIDTH, i * screenHeight / BOARD_HEIGHT, screenWidth / BOARD_WIDTH, screenHeight / BOARD_HEIGHT, RED);
        }
    }
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        DrawLine(i * screenWidth / BOARD_WIDTH, 0, i * screenWidth / BOARD_WIDTH, screenHeight, WHITE);
    }
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        DrawLine(0, i * screenHeight / BOARD_HEIGHT, screenWidth, i * screenHeight / BOARD_HEIGHT, WHITE);
    }
}

int changeTurn (int turn){
    return turn == 1 ? 2 : 1;
}

int checkMove (int board[][BOARD_WIDTH], int level[BOARD_WIDTH], int turn){
    if (!IsCursorOnScreen())
        return turn;
    if (!IsMouseButtonPressed(0))
        return turn;
    int pos = GetMouseX() / (screenWidth / BOARD_WIDTH);
    if (level[pos] > BOARD_HEIGHT - 1)
        return turn;
    board[BOARD_HEIGHT - level[pos] - 1][pos] = turn;
    level[pos]++;
    return changeTurn(turn);
}

void initBoard (int board[][BOARD_WIDTH], int level[BOARD_WIDTH]){
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            board[i][j] = 0;
        }
    }
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        level[i] = 0;
    }
}

bool checkWin (int board[][BOARD_WIDTH]){
    //Check rows
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3] && board[i][j] != 0)
                return true;
        }
    }
    //check columns
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[j][i] == board[j + 1][i] && board[j][i] == board[j + 2][i] && board[j][i] == board[j + 3][i] && board[j][i] != 0)
                return true;
        }
    }
    //check diagonals
    for (int i = 0; i < 4; ++i) {
        for (int j = 3; j < 6; ++j) {
            if (board[j][i] == board[j - 1][i + 1] && board[j][i] == board[j - 2][i + 2] && board[j][i] == board[j - 3][i + 3] && board[j][i] != 0)
                return true;
            if (board[j][BOARD_WIDTH - i - 1] == board[j - 1][BOARD_WIDTH - i - 1 - 1] && board[j][BOARD_WIDTH - i - 1] == board[j - 2][BOARD_WIDTH - i - 1 - 2] && board[j][BOARD_WIDTH - i - 1] == board[j - 3][BOARD_WIDTH - i - 1 - 3] && board[j][BOARD_WIDTH - i - 1] != 0)
                return true;
//            if (board[5][6] == board[4][5] && board[5][6] == board[3][4] && board[5][6] == board[2][3] && board[5][6] != 0)
//                return true;
        }
    }
    return false;
}

bool checkTie (int board[][BOARD_WIDTH]){
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        for (int j = 0; j < BOARD_WIDTH; ++j) {
            if (board[i][j] == 0)
                return false;
        }
    }
    return true;
}

void writeWinMessage (int player){
    player = changeTurn(player);
    std::string win_message = "Ha vinto il giocatore ";
    win_message += (char)(player + '0');
    DrawText(win_message.c_str(), 325, screenHeight / 2 - 50, 30, GREEN);
}

void writeTieMessage (){
    DrawText("Pareggio", 400, screenHeight / 2 - 50, 30, GREEN);
}

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "Connect 4");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    int board[BOARD_HEIGHT][BOARD_WIDTH];
    int level[BOARD_WIDTH];

    initBoard(board, level);

    int player = 1;

    bool win = false;
    bool tie = false;

//    board[0][0] = 1;
//    board[1][2] = 2;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (!win)
            player = checkMove(board, level, player);
        win = checkWin(board);
        tie = checkTie(board);
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        drawBoard(board);
        if (win)
            writeWinMessage(player);
        else if (tie)
            writeTieMessage();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

