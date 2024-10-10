#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void print_board(int const *, int);
int adj_states(int const *, int, int, int);
int *get_board(int);
int *update_board(int *, int);

// Preset initial states
enum Shape {
    Blinker,
    Glider,
    // TODO: toad
    Toad
};

void fill_board(int *, int, enum Shape);

int main(void) {
    const size_t size = 10;
    int *board = get_board(size);

    if (board) {
        fill_board(board, size, 1);
        print_board(board, size);

        for (int a = 0; a < 1000; a++) {
            Sleep(200);
            board = update_board(board, size);
            system("cls");
            print_board(board, size);
        }

        free(board);
    } else {
        fprintf(stderr, "%s", "Error during memory allocation.\n");
    }
    return EXIT_SUCCESS;
}

int *get_board(int const size) {
    return calloc(size*size, sizeof(int));
}

void print_board(int const *board, int const size) {
    // i -> cols, j-> rows
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d  ", board[i*size + j]);
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
int *update_board(int *board, int const size) {
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

    free(board);
    return next_board;
}