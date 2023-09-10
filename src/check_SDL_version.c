#include "check_SDL_version.h"
#include "SDL_version.h"
// #include "SDL_mixer.h"
// #include "SDL_image.h"
#include<stdio.h>

// function to check SDL versions - takes no input currently
void Check_SDL_Version(){

    SDL_version main_version;
    // SDL_version image_version;
    // SDL_version mixer_version;

    SDL_VERSION(&main_version);
    // SDL_IMAGE_VERSION(&image_version);
    // SDL_MIXER_VERSION(&mixer_version);
    
    printf("SDL Version: %i.%i.%i\n", main_version.major, main_version.minor, main_version.patch);
    // printf("SDL_Image Version: %i.%i.%i\n", image_version.major, image_version.minor, image_version.patch);
    // printf("SDL_Mixer Version: %i.%i.%i\n", mixer_version.major, mixer_version.minor, mixer_version.patch);

}