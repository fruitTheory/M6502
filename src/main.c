#include <stdio.h>
#include "config.h"
#include "check_SDL_version.h"
#include "MOS_6502.h"
#include "MOS_6502_stack.h"

int main(int argc, char* argv[]){

    printf("Hello World!\n");
    Check_SDL_Version();

    printf("total mem: %i\n", max_memory);

    struct MOS_6502 MOS_6502;
    MOS_6502_stack_push(&MOS_6502);
    MOS_6502_stack_pop(&MOS_6502);

    return EXIT_SUCCESS;

}