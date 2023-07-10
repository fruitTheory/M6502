#include <stdio.h>
#include "config.h"
#include "check_SDL_version.h"
#include "MOS_6502.h"
#include "MOS_6502_stack.h"
#include "load_binary.h"

int main(int argc, char* argv[]){

    load_binary(argc, argv);

    printf("Hello World!\n");
    Check_SDL_Version();

    printf("total memory: %i\n", max_memory);

    struct MOS_6502 MOS_6502;

    // TO FIX TMRW -- CAPPED AT unsigned char(255) cant init to 511(0x1FF)
    MOS_6502_stack_init(&MOS_6502);

    printf("%i\n", MOS_6502.registers.SP);

    //0x0F, 0x8F;

    MOS_6502.registers.SP = 5;

    printf("%i\n", MOS_6502.registers.SP);

    MOS_6502_init(&MOS_6502);

    //printf("addy: %p\n", (void*)&MOS_6502);
    MOS_6502_stack_push(&MOS_6502, 0x100);
    MOS_6502_stack_pop(&MOS_6502, 0x100);

    return EXIT_SUCCESS;
}