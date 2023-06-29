#include <stdio.h>
#include "config.h"
#include "check_SDL_version.h"
#include "MOS_6502.h"


int main(int argc, char* argv[]){

    printf("Hello World!\n");
    Check_SDL_Version();

    printf("total mem: %i\n", max_memory);

}