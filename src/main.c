#include<stdio.h>

#define SDL_MAIN_HANDLED
#include "SDL2\SDL.h"
#include "SDL2\SDL_video.h"
#include "SDL2\SDL_audio.h"
#include "SDL2\SDL_mixer.h"
#include "SDL2\SDL_image.h"

#include "version_check.h"


int main(int argc, char* argv[]){

    // Init SDL video and audio flags
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO)==0){
        printf("Success! (Initialised)\n");
    };

    // Set log check SDL version
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
    CheckVersion();

    // Init variables of custom types 
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;

    // Create window and Renderer - returns 0 on success
    int window_render = SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);

    // Init window info
    SDL_SetWindowTitle(window, "Mi Direct3D Window");
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetRenderDrawColor(renderer, 128, 0, 128, SDL_ALPHA_OPAQUE);

    // To show color and update render both these functions are needed in this order
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_bool running = SDL_TRUE;
    
    // Starting loop for window display
    if(window_render != 0){
        printf("Window renderer failed: %s\n", SDL_GetError());
        return -1;
    } else {
        while(running){
            // Poll for any events - ex: mouse events, keyboard events, etc.
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    running = SDL_FALSE;
                }

            Mix_Chunk* chunk = Mix_LoadWAV("./audio/2Pac.wav");
            if (chunk==NULL){
                printf("No file found. Fuck you! \n");
                // Mix_GetError();
                //break;
            }

            if(Mix_OpenAudio(44100, AUDIO_S16SYS, 1, 4096) < 0){
                printf("Audio not load I guess");
            };
            Mix_PlayChannel(-1, chunk, 0);
                 


            SDL_ShowWindow(window);
            
            }
        }

    }


    /*--------------Surface----------------*/
    const char *image_path = "./images/CameraVector.bmp";
    SDL_Surface *image = SDL_LoadBMP(image_path);

    if (!image) {
        printf("Failed to load image at %s: %s\n", image_path, SDL_GetError());
        return -1;
    };

    // Make sure to eventually release resources
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;

}