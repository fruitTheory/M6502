#include <stdio.h>
#include "config.h"
#include "check_SDL_version.h"
#include "MOS_6502.h"
#include "MOS_6502_stack.h"
#include "load_binary.h"
#include <stdlib.h>


int main(int argc, char* argv[]){

    // first thing
    Check_SDL_Version();
    printf("total memory: %i\n", max_memory);

    // initialized MOS_6502 variable
    struct MOS_6502 MOS_6502;
    MOS_6502_init(&MOS_6502);
    
    // attempt load program into memory
    uchar8_t* program = load_program(argc, argv);
    print_file_info(argc, argv);
    ushort16_t program_size = get_file_size(argv);
    printf("program size: %i\n", program_size);
    program_file_store(&MOS_6502, program, program_size);
    free(program);

    // print stuff belore
    printf("size of MOS_6502: %lli\n", sizeof(MOS_6502));
    MOS_6502_stack_init(&MOS_6502);
    printf("stack unitialized: %i\n", MOS_6502.registers.SP);
    MOS_6502.registers.SP = 5;
    printf("stack initialized: %i\n", MOS_6502.registers.SP);

    // push and pop to stack
    MOS_6502_stack_push(&MOS_6502, 0x100);
    MOS_6502_stack_pop(&MOS_6502, 0x100);

    return EXIT_SUCCESS;
}