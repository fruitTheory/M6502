#include <stdio.h>
#include "config.h"
#include "check_SDL_version.h"
#include "M6502.h"
#include "M6502_stack.h"
#include "load_binary.h"
#include <stdlib.h>
#include "memory.h"


int main(int argc, char* argv[]){

    // first thing
    //Check_SDL_Version();
    printf("total system memory: %i\n", max_memory);
    flag_bit -= 5;
    printf("flag bit: %i\n", flag_bit);
    // initialized M6502 variable
    struct M6502 M6502;
    M6502_init(&M6502);
    printf("size of M6502 struct: %lli\n", sizeof(struct M6502));
    
    // load program into memory - array of uchar8_t
    uchar8_t* program = load_program(argc, argv);
    print_program_info(argc, argv);
    ushort16_t program_size = get_program_size(argv);
    printf("program size: %i bytes\n", program_size);
    M6502_store_program(&M6502, program, program_size);
    free(program);

    // print stuff belore
    M6502_stack_init(&M6502);
    printf("stack unitialized: %i\n", M6502.registers.SP);
    M6502.registers.SP = 5;
    printf("stack initialized: %i\n", M6502.registers.SP);

    // push and pop to stack
    M6502_stack_push(&M6502, 0x100);
    M6502_stack_pop(&M6502, 0x100);

    return EXIT_SUCCESS;
}