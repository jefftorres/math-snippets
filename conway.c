#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 960

// Preset initial states
enum Shape {
    Blinker,
    Glider,
    // TODO: toad
    Toad
};

int *get_board(int);
void print_board(int const *, int);
void fill_board(int *, int, enum Shape);

int adj_states(int const *, int, int, int);
int *update_board(int const *, int);


bool init();
bool load();
SDL_Texture *load_texture(char const *path);
bool loop();
void kill();

// Pointers to window and surface
SDL_Surface *g_surface = NULL;
SDL_Surface *mandelbrot = NULL;
SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
SDL_Texture *g_texture = NULL;

int main(int argc, char** argv) {
    // Working code for console ver. of Conway
    // const size_t size = 10;
    // int *board = get_board(size);
    //
    // if (board) {
    //     fill_board(board, size, 0);
    //     print_board(board, size);
    //
    // for (int a = 0; a < 1000; a++) {
    //     Sleep(250);
    //     board = update_board(board, size);
    //     system("cls");
    //     print_board(board, size);
    // }
    //
    //     free(board);
    // } else {
    //     fprintf(stderr, "%s", "Error during memory allocation.\n");
    // }

    // SDL Init
    if (!init()) return 1;
    if (!load()) return 1;

    // SDL_Rect viewport = {0, 0, 640/2, 480/2};
    // SDL_RenderSetViewport(g_renderer, &viewport);

    // SDL_RenderCopy(g_renderer, mandelbrot, NULL, NULL);

    // blits image when loaded as IMG
    // SDL_BlitSurface(mandelbrot, NULL, g_surface, NULL);

    while(loop()) {
        SDL_Delay(10);

        // Keep checking about render
        // https://lazyfoo.net/tutorials/SDL/07_texture_loading_and_rendering/index.php
        SDL_RenderClear(g_renderer);
        SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);
        SDL_RenderPresent(g_renderer);
    }

    kill();
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

// Initialize SDL.
bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {

        printf("Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    // Creates the window
    g_window = SDL_CreateWindow("Conway", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (g_window == NULL) {
        printf("Error creating window: %s", SDL_GetError());
        return false;
    }

    // Getting the surface
    g_surface = SDL_GetWindowSurface(g_window);
    if (g_surface == NULL) {
        printf("Error getting surface: %s", SDL_GetError());
        return false;
    }

    return true;
}

// Loads assets
bool load() {
    g_renderer = SDL_CreateSoftwareRenderer(g_surface);
    if (g_renderer == NULL) {
        printf("Renderer could not be created! SDL error: %s", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(g_renderer, 0xff, 0xff, 0xff, 0xff);

    // Load as texture
    g_texture = load_texture("mandelbrot.jpg");
    if (g_texture == NULL) {
        printf("Failed to load texture image!\n");
        return false;
    }

    // Load as IMG
    // mandelbrot = IMG_Load("mandelbrot.jpg");
    // if (mandelbrot == NULL) {
    //     printf("Unable to load image %s, SDL error: %s\n", "mandelbrot.jpg", SDL_GetError());
    //     return false;
    // }

    return true;
}

SDL_Texture *load_texture(char const *path) {
    SDL_Texture *new_texture = NULL;
    SDL_Surface *loaded_surface = IMG_Load(path);

    if (loaded_surface == NULL) {
        printf("Unable to load image %s, SDL error: %s\n", path, SDL_GetError());
    } else {
        new_texture = SDL_CreateTextureFromSurface(g_renderer, loaded_surface);
        if (new_texture == NULL) {
            printf("Unable to create texture from %s! SDL error: %s", path, SDL_GetError());
        }

        SDL_FreeSurface(loaded_surface);
    }

    return new_texture;
}

// TODO Need to check working keystrokes
bool loop() {
    static bool square;
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        switch (e.type) {
            case SDL_QUIT:
                return false;
            case SDL_KEYUP:
                square = true;
            break;
            case SDL_KEYDOWN:
                square = false;
            break;
            case SDL_MOUSEMOTION:
                break;
            default:
                break;
        }
    }

    if (square) {
        // SDL_FillRect(g_surface, NULL, SDL_MapRGB(g_surface->format, 255, 255, 255));
    }

    // Update window
    SDL_UpdateWindowSurface(g_window);
    return true;
}

// Free loaded assets and quit SDL
void kill() {
    SDL_FreeSurface(mandelbrot);
    mandelbrot = NULL;
    SDL_FreeSurface(g_surface);
    g_surface = NULL;

    SDL_DestroyRenderer(g_renderer);
    g_renderer = NULL;
    SDL_DestroyTexture(g_texture);
    g_texture = NULL;
    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}