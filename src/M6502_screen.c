#include "M6502_screen.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <stdbool.h>



void func(){
    const char* title = "My Window";

    SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    SDL_Event event;
    SDL_Window* window;

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 256, 240, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    // Text setup
    TTF_Font* font = TTF_OpenFont("./font/DejaVuSans.ttf", 14);
    SDL_Color textColor = {255, 255, 255, 128};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Hello World!", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = {100, 10, textSurface->w, textSurface->h}; // size of a rectangle x, y - w, h

    bool running = true;
    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type ==SDL_QUIT) running = false;
        }

        //SDL_ShowWindow(window);
        SDL_SetRenderDrawColor(renderer, 0, 100, 128, 255); // sets color
        SDL_RenderClear(renderer); // clear current target with draw color
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect); // presents text
        SDL_RenderPresent(renderer); // presents render

    }

    // cleanup
    //SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer); // destroy renderer and associated textures
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

