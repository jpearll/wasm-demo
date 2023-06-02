
// Import necessary libraries
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>

/**
This header file provides a collection of functions, macros, and variables that help developers interface 
with Web APIs and manage the life cycle of the WebAssembly application.

One of the key features provided by emscripten.h is the ability to set the main loop for the program, 
which is necessary for creating interactive applications such as games or graphical applications.
**/

// Define the dimensions of the window
#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480

// Define the size of the sprite
const unsigned int size = 64;

// Initialize SDL window and renderer
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

// Initialize the velocity of the sprite and its size
SDL_Point velocity = {0, 0};
SDL_Rect sprite = {0, 0, size, size};
SDL_Texture *texture = NULL;

// Function to initialize SDL and create window and renderer
bool init() {
     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);
    if (window == NULL | renderer == NULL) {
        return false;
    }

    return true;
}

// Function to load textures
void load_textures() {
    SDL_Surface *surface = IMG_Load("assets/texture.png");
    if (!surface) {
        printf("%s\n", IMG_GetError());
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0x75, 0xFF, 0xFF));
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

// Function to process events like key press
void process_event(SDL_Event *event) {
    SDL_Keycode key = event->key.keysym.sym;

    if (event->key.type == SDL_KEYDOWN) {
        if (key == SDLK_LEFT || key == SDLK_RIGHT) {
            velocity.x = key == SDLK_LEFT ? -1 : 1;
        }
        if (key == SDLK_UP || key == SDLK_DOWN) {
            velocity.y = key == SDLK_UP ? -1 : 1;
        }
    }
    if (event->key.type == SDL_KEYUP) {
        velocity.x = 0;
        velocity.y = 0;
    }
}

// Function to process input from the user
void process_input() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        process_event(&event);
    }
}

// Function to update the sprite position
void update() {
    sprite.x += velocity.x;
    sprite.y += velocity.y;
}

// Function to draw the sprite on the renderer
void draw() {
    SDL_RenderCopy(renderer, texture, NULL, &sprite);
}

// Main game loop
void main_loop() {
    process_input();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    update();
    draw();

    SDL_RenderPresent(renderer);
}

// Function to destroy the window and quit SDL
void destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Main function
int main() {
    // Initialize SDL and load textures
    init();
    load_textures();

    // Center the sprite in the window
    sprite.x = (WINDOW_WIDTH - size) / 2;
    sprite.y = (WINDOW_HEIGHT - size) / 2;

    // Set the main loop and start the game
    // allows you to set a function that Emscripten should call repeatedly, 
    // creating a game or application loop. 
    // This is used to keep the application running and 
    // responding to user input.

    emscripten_set_main_loop(main_loop, -1, 1);

    // Destroy the window and quit SDL when the game is finished
    destroy();
    return EXIT_SUCCESS;
}
