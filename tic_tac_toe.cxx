#include <SDL2/SDL.h>
#include <iostream>
#include <limits>
#include <cmath>

using namespace std;

const char PLAYER = 'X';
const char AI = 'O';
const char EMPTY = ' ';

char board[3][3] = {
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY},
    {EMPTY, EMPTY, EMPTY}
};

// Game logic functions
bool isMovesLeft() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == EMPTY)
                return true;
    return false;
}

int evaluate() {
    for (int row = 0; row < 3; row++) {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2]) {
            if (board[row][0] == PLAYER)
                return -10;
            else if (board[row][0] == AI)
                return 10;
        }
    }

    for (int col = 0; col < 3; col++) {
        if (board[0][col] == board[1][col] && board[1][col] == board[2][col]) {
            if (board[0][col] == PLAYER)
                return -10;
            else if (board[0][col] == AI)
                return 10;
        }
    }

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER)
            return -10;
        else if (board[0][0] == AI)
            return 10;
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER)
            return -10;
        else if (board[0][2] == AI)
            return 10;
    }

    return 0;
}

int minimax(bool isMax) {
    int score = evaluate();

    if (score == 10)
        return score;

    if (score == -10)
        return score;

    if (!isMovesLeft())
        return 0;

    if (isMax) {
        int best = numeric_limits<int>::min();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = AI;
                    best = max(best, minimax(false));
                    board[i][j] = EMPTY;
                }
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER;
                    best = min(best, minimax(true));
                    board[i][j] = EMPTY;
                }
            }
        }
        return best;
    }
}

pair<int, int> findBestMove() {
    int bestVal = numeric_limits<int>::min();
    pair<int, int> bestMove = {-1, -1};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = AI;
                int moveVal = minimax(false);
                board[i][j] = EMPTY;

                if (moveVal > bestVal) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
        }
    }

    return bestMove;
}

bool checkWin() {
    int score = evaluate();
    if (score == 10) {
        cout << "AI wins!\n";
        return true;
    } else if (score == -10) {
        cout << "Player wins!\n";
        return true;
    } else if (!isMovesLeft()) {
        cout << "It's a draw!\n";
        return true;
    }
    return false;
}

void drawEllipse(SDL_Renderer* renderer, int cx, int cy, int rx, int ry, SDL_Color color) {
    int x = 0;
    int y = ry;
    int a2 = rx * rx;
    int b2 = ry * ry;
    int err = 0;
    int dx = 2 * b2 * x;
    int dy = 2 * a2 * y;
    int d = b2 - (a2 * ry) + (0.25 * a2);
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Draw ellipse using the midpoint algorithm
    while (dx < dy) {
        SDL_RenderDrawPoint(renderer, cx + x, cy - y);
        SDL_RenderDrawPoint(renderer, cx - x, cy - y);
        SDL_RenderDrawPoint(renderer, cx + x, cy + y);
        SDL_RenderDrawPoint(renderer, cx - x, cy + y);
        
        x++;
        dx += 2 * b2;
        if (d >= 0) {
            y--;
            dy -= 2 * a2;
            d += a2 - dy;
        }
        d += b2 + dx;
    }
    d = b2 * (x + 0.5) * (x + 0.5) + a2 * (y - 1) * (y - 1) - a2 * b2;
    while (y >= 0) {
        SDL_RenderDrawPoint(renderer, cx + x, cy - y);
        SDL_RenderDrawPoint(renderer, cx - x, cy - y);
        SDL_RenderDrawPoint(renderer, cx + x, cy + y);
        SDL_RenderDrawPoint(renderer, cx - x, cy + y);
        
        y--;
        dy -= 2 * a2;
        if (d <= 0) {
            x++;
            dx += 2 * b2;
            d += b2 + dx;
        }
        d += a2 - dy;
    }
}

void drawBoard(SDL_Renderer* renderer) {
    int cellSize = 200;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Draw grid
    for (int i = 1; i < 3; ++i) {
        SDL_RenderDrawLine(renderer, 0, i * cellSize, 600, i * cellSize);
        SDL_RenderDrawLine(renderer, i * cellSize, 0, i * cellSize, 600);
    }

    // Draw Xs and Os
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int x = j * cellSize;
            int y = i * cellSize;
            if (board[i][j] == PLAYER) {
                // Draw X
                SDL_RenderDrawLine(renderer, x + 20, y + 20, x + cellSize - 20, y + cellSize - 20);
                SDL_RenderDrawLine(renderer, x + cellSize - 20, y + 20, x + 20, y + cellSize - 20);
            } else if (board[i][j] == AI) {
                // Draw O
                drawEllipse(renderer, x + cellSize / 2, y + cellSize / 2, cellSize / 4, cellSize / 4, {0, 0, 255, 255});
            }
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Tic-Tac-Toe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int row = y / 200;
                int col = x / 200;

                if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == EMPTY) {
                    board[row][col] = PLAYER;

                    if (!checkWin()) {
                        pair<int, int> bestMove = findBestMove();
                        board[bestMove.first][bestMove.second] = AI;
                        checkWin();
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        drawBoard(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}