

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

const int DIM = 3;
const int screenWidth = 800;
const int screenHeight = 800;

struct Position{
    int row;
    int column;
};

void drawX (int x, int y){
    DrawLine(x + 60, y + 60, x + screenWidth / 3 - 60, y + screenHeight / 3 - 60, WHITE);
    x += screenWidth / 3;
    DrawLine(x - 60, y + 60, x - screenWidth / 3 + 60, y + screenHeight / 3 - 60, WHITE);
}

void drawO (int x, int y){
    x += screenWidth / 6;
    y += screenHeight / 6;
    DrawCircle(x, y, screenWidth / 6 - 60, WHITE);
    DrawCircle(x, y, screenWidth / 6 - 60 - 1, BLACK);
}

void drawBoard (char board[][DIM]){
    for (int i = 0; i < DIM; ++i) {
        DrawLine(0, i * screenHeight / 3, screenWidth, i * screenHeight / 3, WHITE);
    }
    for (int i = 0; i < DIM; ++i) {
        DrawLine(i * screenWidth / 3, 0, i * screenWidth / 3, screenHeight, WHITE);
    }
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            if (board[i][j] == 'x')
                drawX(i * screenWidth / 3, j * screenHeight / 3);
            else if (board[i][j] == 'o')
                drawO(i * screenWidth / 3, j * screenHeight / 3);
        }
    }
}

bool checkMove (char board[][DIM], bool turn){
    if (!turn)
        return turn;
    if (!IsCursorOnScreen())
        return turn;
    if (!IsMouseButtonPressed(0))
        return turn;
    int x = GetMouseX(), y = GetMouseY();
    int x_pos = x / (screenWidth / DIM), y_pos = y / (screenHeight / DIM);
    if (board[x_pos][y_pos] != ' ')
        return turn;
    board[x_pos][y_pos] = 'x';
    return !turn;
}

bool checkWin (char board[][DIM]){
    //controllo righe e colonne
    for (int i = 0; i < DIM; ++i) {
        if (board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] != ' ')
            return true;
        if (board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] != ' ')
            return true;
    }
    //controllo diagonali
    if (board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] != ' ')
        return true;
    if (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] != ' ')
        return true;
    return false;
}

bool checkTie (char board[][DIM]){
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            if (board[i][j] == ' ')
                return false;
        }
    }
    return true;
}

void writeWinMessage (bool turn){
    char player;
    if (!turn)
        player = '1';
    else
        player = '2';
    std::string win_message = "Ha vinto il giocatore ";
    win_message += player;
    DrawText(win_message.c_str(), 180, screenHeight / 2 - 30, 40, WHITE);
}

void writeTieMessage (){
    DrawText("Pareggio", 300, screenHeight / 2 - 30, 40, WHITE);
}

void boardInit (char board[][DIM]){
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            board[i][j] = ' ';
        }
    }
}

int valueMove (char board[][DIM], int depth, bool turn){
    //base case if the board is in a terminal state
    if (checkWin(board)){
        if (!turn)
            return -100 + depth;
        return 100 - depth;
    }
    if (checkTie(board))
        return 0;
    //if is minimizer player
    if (turn) {
        int bestValue = 10000;
        char player = 'x';
        for (int i = 0; i < DIM; ++i) {
            for (int j = 0; j < DIM; ++j) {
                if (board[i][j] != ' ')
                    continue;
                board[i][j] = player;
                int value = valueMove(board, depth + 1, !turn);
                bestValue = std::min(value, bestValue);
                board[i][j] = ' ';
            }
        }
        return bestValue;
    }
    //if is maximizer turn
    int bestValue = -10000;
    char player = 'o';
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            if (board[i][j] != ' ')
                continue;
            board[i][j] = player;
            int value = valueMove(board, depth + 1, !turn);
            bestValue = std::max(value, bestValue);
            board[i][j] = ' ';
        }
    }
    return bestValue;
}

void nextMoveAI (char board[][DIM]){
    int bestValue = -10;
    Position pos = {-1, -1};
    for (int i = 0; i < DIM; ++i) {
        for (int j = 0; j < DIM; ++j) {
            if (board[i][j] != ' ')
                continue;
            board[i][j] = 'o';
            int value = valueMove(board, 0, true);
            if (value > bestValue){
                pos.row = i;
                pos.column = j;
                bestValue = value;
            }
            board[i][j] = ' ';
        }
    }
    board[pos.row][pos.column] = 'o';
}

void checkReset (char board[][DIM], bool &win, bool &tie, bool &player){
    if (!IsKeyDown(KEY_SPACE))
        return;
    boardInit(board);
    win = false;
    tie = false;
    player = true;
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    InitWindow(screenWidth, screenHeight, "Tic Tac Toe");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    char board[DIM][DIM];
    boardInit(board);

    bool turn = false; //true = Player 1, false = Player 2
    bool win;
    bool tie;

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        turn = checkMove(board, turn);
        win = checkWin(board);
        tie = checkTie(board);
        if (!turn) {
            nextMoveAI(board);
            turn = !turn;
        }
        if (win || tie){
            checkReset(board, win, tie, turn);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        if (win)
            writeWinMessage(turn);
        else if (tie)
            writeTieMessage();
        else
            drawBoard(board);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

