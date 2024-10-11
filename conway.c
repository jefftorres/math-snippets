#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <SDL.h>

// Preset initial states
enum Shape {
    Blinker,
    Glider,
    // TODO: toad
    Toad
};

void print_board(int const *, int);
int adj_states(int const *, int, int, int);
int *get_board(int);
int *update_board(int const *, int);
void fill_board(int *, int, enum Shape);

bool init();
void kill();
bool load();
bool loop();

// Pointers to window and surface
SDL_Surface *winSurface = NULL;
SDL_Window *window = NULL;

int main(int argc, char** argv) {
    // const size_t size = 10;
    // int *board = get_board(size);
    //
    // if (board) {
    //     fill_board(board, size, 0);
    //     print_board(board, size);

    if (!init()) return 1;
    if (!load()) return 1;

    while(loop()) {
        SDL_Delay(10);
    }

    kill();

    // for (int a = 0; a < 1000; a++) {
    //     Sleep(250);
    //     board = update_board(board, size);
    //     system("cls");
    //     print_board(board, size);
    // }

    //     free(board);
    // } else {
    //     fprintf(stderr, "%s", "Error during memory allocation.\n");
    // }
    return EXIT_SUCCESS;
}

int *get_board(int const size) {
    return calloc(size*size, sizeof(int));
}

void print_board(int const *board, int const size) {
    // i -> cols, j-> rows
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", board[i*size + j]);
        }
        printf("\n");
    }
}

void fill_board(int *board, int const size, enum Shape const s) {
    switch (s) {
        // Generates a blinker
        case 0:
            for (int i = size/2-1; i <= size/2+1; i++) {
                board[i*size + size/2] = true;
            }
        // Generates a glider
        case 1:
            for (int i = size/2-1; i <= size/2+1; i++) {
                for (int j = size/2-1; j <= size/2+1; j++) {
                    if (j == size/2+1) board[i*size + j] = true;
                    if (i == j+1) board[i*size + j] = true;
                }
            }
        default:
            // TODO: Could fill board with a random state
            break;
    }
}

// Check for amount of alive cells adjacent to given (x,y) position
int adj_states(int const *board, int const x, int const y, int const size) {
    int alive = 0;

    int const xl = x == 0 ? size-1 : x-1;
    int const xu = x == size-1 ? 0 : x+1;
    int const yl = y == 0 ? size-1 : y-1;
    int const yu = y == size-1 ? 0 : y+1;

    if (board[x*size + yl]) alive++;
    if (board[x*size + yu]) alive++;
    if (board[xl*size + y]) alive++;
    if (board[xu*size + y]) alive++;
    if (board[xl*size + yl]) alive++;
    if (board[xl*size + yu]) alive++;
    if (board[xu*size + yl]) alive++;
    if (board[xu*size + yu]) alive++;
    // printf("Alive adjacents for cell %d (%d, %d): %d.\n", x*size + y, x, y, alive);

    return alive;
}

// Next board state
int *update_board(int const *board, int const size) {
    int *next_board = get_board(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int const adjs = adj_states(board, i, j, size);

            if (board[i*size + j]) {
                if (adjs == 2 || adjs == 3) next_board[i*size + j] = true;
            } else {
                if (adjs == 3) next_board[i*size + j] = true;
            }
        }
    }

    // free(board);
    return next_board;
}

bool loop() {
    static bool square;
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        switch (e.type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYUP:
                square = true;
            case SDL_KEYDOWN:
                square = false;
            case SDL_MOUSEMOTION:
                break;
        }
    }

    if (square) {
        SDL_Rect sq;
        sq.x = 160;
        sq.y = 120;
        sq.w = 320;
        sq.h = 240;

    }

    // Update window
    SDL_UpdateWindowSurface(window);
    return true;
}

bool load() {
    SDL_FillRect(winSurface, NULL, SDL_MapRGB(winSurface->format, 255, 255, 255));
    return true;
}

bool init() {
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

        printf("Error initializing SDL: %s\n", SDL_GetError());
        system("pause");
        return false;
    }

    // Creates the window
    window = SDL_CreateWindow("Conway", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1280, 720, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Error creating window: %s", SDL_GetError());
        system("pause");
        return false;
    }

    // Getting the surface
    winSurface = SDL_GetWindowSurface(window);
    if (!winSurface) {
        printf("Error getting surface: %s", SDL_GetError());
        system("pause");
        return false;
    }

    return true;
}

void kill() {
    SDL_FreeSurface(winSurface);

    SDL_DestroyWindow(window);
    SDL_Quit();
}
