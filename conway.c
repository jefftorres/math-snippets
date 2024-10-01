#include "stdio.h"
#include "stdlib.h"

void fill(char **, int);
void print(char **, int);

int main(void) {
    unsigned int size = 8;

    char **board = malloc(size * sizeof(char *));
    for (int i=0; i<size; i++) {
        board[i] = (char *) calloc(size, sizeof(char));
    }

    if (board) {
        fill(board, size);
        print(board, size);

        for (int i=0; i<size; i++) {
            free(board[i]);
        }
        free(board);
    }

    return EXIT_SUCCESS;
}

void fill(char **board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i*size + j] = "_";
        }
    }
}

void print(char **board, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%s", board[i*size + j]);
        }
        printf("\n");
    }
}