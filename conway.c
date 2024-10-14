#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// Preset initial states
typedef enum {
    Blinker,
    Glider,
    Pentomino
} Shape;

int *get_board(size_t);
void print_board(int const*, size_t);
void fill_board(int*, size_t, Shape);

int adj_states(int const*, int, int, size_t);
int *update_board(int const*, size_t);

bool init();
bool load();
SDL_Texture *load_texture(char const*);
bool loop();
void kill();

// Pointers to window and surface
SDL_Surface *g_surface = NULL;
SDL_Surface *mandelbrot = NULL;
SDL_Window *g_window = NULL;
SDL_Renderer *g_renderer = NULL;
SDL_Texture *g_texture = NULL;

int main(int argc, char **argv) {
    size_t const size = 10;
    int *board = get_board(size);

    if (board) {
        fill_board(board, size, 1);
        //     print_board(board, size);
    }

    // SDL Init
    if (!init()) return 1;
    if (!load()) return 1;

    // SDL_Rect viewport = {0, 0, 640/2, 480/2};
    // SDL_RenderSetViewport(g_renderer, &viewport);

    // SDL_RenderCopy(g_renderer, mandelbrot, NULL, NULL);

    // blits image when loaded as IMG
    // SDL_BlitSurface(mandelbrot, NULL, g_surface, NULL);

    // drawing grid
    int const delta = SCREEN_HEIGHT/size * 0.95;
    SDL_Rect grid;
    grid.w = delta;
    grid.h = delta;
    int const dist = delta*size/2;

    int const yl = SCREEN_HEIGHT/2 - dist;
    int const yu = SCREEN_HEIGHT/2 + dist;
    int const xl = SCREEN_WIDTH/2 - dist;
    int const xu = SCREEN_WIDTH/2 + dist;

    while(loop()) {
        SDL_SetRenderDrawColor(g_renderer, 0x11, 0x11, 0x11, 0xff);
        SDL_RenderClear(g_renderer);

        // Draw blue horizontal line
        // SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0xcc, 0xff);
        // SDL_RenderDrawLine(g_renderer, 0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2);

        // Renders the mandelbrot
        // SDL_RenderCopy(g_renderer, g_texture, NULL, NULL);

        int z1 = 0;
        for (int dy=xl; dy<xu; dy+=delta) {
            int z2 = 0;
            grid.x = dy;

            for (int dx=yl; dx<yu; dx+=delta) {
                grid.y = dx;
                if (board[z1*size + z2]) {
                    // #2d8a91
                    SDL_SetRenderDrawColor(g_renderer, 0x2d, 0x8a, 0x91, 0xff);
                    SDL_RenderFillRect(g_renderer, &grid);
                }
                z2++;
                // gray
                SDL_SetRenderDrawColor(g_renderer, 0xdc, 0xdc, 0xdc, 0xff);
                SDL_RenderDrawRect(g_renderer, &grid);
            }
            z1++;
        }
        SDL_RenderPresent(g_renderer);
        board = update_board(board, size);
        SDL_Delay(80);
    }

    free(board);
    kill();
    return EXIT_SUCCESS;
}

int *get_board(size_t const size) {
    return calloc(size*size, sizeof(int));
}

void print_board(int const *board, size_t const size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", board[i*size + j]);
        }
        printf("\n");
    }
}

void fill_board(int *board, size_t const size, Shape const s) {
    int count = 0;
    switch (s) {
        // Generates a blinker
        case 0:
            for (int i=size/2-1; i <= size/2+1; i++) {
                board[i*size + size/2] = true;
                count++;
            }
            break;
        // Generates a glider
        case 1:
            for (int i=size/2-1; i <= size/2+1; i++) {
                for (int j=size/2-1; j <= size/2+1; j++) {
                    if (j == size/2+1) board[i*size + j] = true;
                    if (i == j+1)      board[i*size + j] = true;
                    count++;
                }
            }
            break;
        case 2:
            for (int i = size/2-1; i <= size/2+1; i++) {
                for (int j=size/2-1; j <= size/2+1; j++) {
                    if (i == size/2)                    board[i*size + j] = true;
                    if (i == size/2-1 && j == size/2)   board[i*size + j] = true;
                    if (i == size/2+1 && j == size/2-1) board[i*size + j] = true;
                }
            }
        default:
            // TODO: Could fill board with a random state
            break;
    }
}

// Check for amount of alive cells adjacent to given (x,y) position
int adj_states(int const *board, int const x, int const y, size_t const size) {
    int alive = 0;

    int const xl = x == 0 ? size-1 : x-1;
    int const xu = x == size-1 ? 0 : x+1;
    int const yl = y == 0 ? size-1 : y-1;
    int const yu = y == size-1 ? 0 : y+1;

    // Any other way to do this?
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
int *update_board(int const *board, size_t const size) {
    int *next_board = get_board(size);

    for (int i=0; i < size; i++) {
        for (int j=0; j < size; j++) {
            int const adjs = adj_states(board, i, j, size);

            if (board[i*size + j]) {
                if (adjs == 2 || adjs == 3) next_board[i*size + j] = true;
            } else {
                if (adjs == 3) next_board[i*size + j] = true;
            }
        }
    }

    return next_board;
}

// Initialize SDL.
bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Error initializing SDL: %s\n", SDL_GetError());
        return false;
    }

    // Creates the window
    g_window = SDL_CreateWindow("Conway", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_CENTERED,
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

    // Load img as texture
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