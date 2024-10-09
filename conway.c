#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int *get_board(int);
void print(int *, int);
int adj_states(int *, int, int, int);

enum Shape {
    Blinker,
    Toad,
    Glider
};

void fill(int *, int, enum Shape);

int main(void) {
    unsigned const int size = 5;
    int *board = get_board(size);

    if (board) {
        fill(board, size, 0);
        print(board, size);

        free(board);
    } else {
        fprintf(stderr, "%s", "Error during memory allocation.\n");
    }

    return EXIT_SUCCESS;
}

int *get_board(int size) {
    return calloc(size*size, sizeof(int));
}

void print(int *board, int size) {
    // i -> cols, j-> rows
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d\t", board[i*size + j]);
        }
        printf("\n");
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            adj_states(board, i, j, size);
        }
    }
}

// For now it generates a blinker around the middle of the board
void fill(int *board, int size, enum Shape s) {
    if (!s) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (i > size/2 - 1 && i < size/2 + 1 && j > size/2 - 2 && j < size/2 + 2) {
                    board[i*size + j] = true;
                } else {
                    board[i*size + j] = false;
                }
            }
        }
    }
}

// Check for amount of alive cells adjacent to given (x,y) position
int adj_states(int *board, int x, int y, int size) {
    int alive = 0;

    int xl = x == 0 ? size-1 : x-1;
    int xu = x == size-1 ? 0 : x+1;
    int yl = y == 0 ? size-1 : y-1;
    int yu = y == size-1 ? 0 : y+1;

    if (board[x*size + yl]) alive++;
    if (board[x*size + yu]) alive++;
    if (board[xl*size + y]) alive++;
    if (board[xu*size + y]) alive++;
    if (board[xl*size + yl]) alive++;
    if (board[xl*size + yu]) alive++;
    if (board[xu*size + yl]) alive++;
    if (board[xu*size + yu]) alive++;

    printf("Alive adjacents for cell %d (%d, %d): %d.\n", x*size + y, x, y, alive);

    return alive;
}