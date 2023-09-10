#include "M6502_screen.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdbool.h>
#include <stdio.h>



void func(){

    const char* title = "My Window";

    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    SDL_Event event;
    SDL_Window* window;
    
    int displays = SDL_GetNumVideoDisplays();
    printf("num displays%i\n", displays);

    // Dev setup to display on specific window, ignore later
    // Dev display resolution - 3840x2160
    SDL_Rect display_bounds;
    SDL_GetDisplayBounds(1, &display_bounds); // Get bounds of the second display
    printf("disp bounds: %i %i\n", display_bounds.y, display_bounds.y);

    window = SDL_CreateWindow(title, display_bounds.x+1600, display_bounds.y+600, 256, 240, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // create renderer for specific window

    // Text setup constants - font type, size, and color
    TTF_Font* font = TTF_OpenFont("./font/DejaVuSans.ttf", 14);
    SDL_Color text_color = {255, 255, 255, 128};

    // Temporary number
    int number=0;

    bool running = true;
    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type ==SDL_QUIT) running = false;
        }

        const bounds = 11;
        char number_str[bounds+1];

        number+=1;
        if(number < 11)
            sprintf(number_str, "%d", number);

        // Surface creation and conversion to texture
        SDL_Surface* text_surface = TTF_RenderText_Solid(font, number_str, text_color);
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_Rect text_rect = {100, 10, text_surface->w, text_surface->h}; // size of a rectangle x, y - w, h

        //SDL_ShowWindow(window);
        SDL_SetRenderDrawColor(renderer, 0, 100, 128, 255); // sets color
        SDL_RenderClear(renderer); // clear current target with draw color
        SDL_RenderCopy(renderer, text_texture, NULL, &text_rect); // presents text
        SDL_RenderPresent(renderer); // presents render

        // Cleanup from new creation
        SDL_FreeSurface(text_surface);
        SDL_DestroyTexture(text_texture);

        SDL_Delay(500);
    }

    // cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer); // destroy renderer and associated textures
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

