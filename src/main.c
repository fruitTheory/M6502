#include <stdio.h>
#include "config.h"
#include "check_SDL_version.h"
#include "M6502.h"
#include "M6502_stack.h"
#include "load_binary.h"
#include <stdlib.h>
#include <memory.h>
#include "M6502_instructions.h"
#include <time.h>


int main(int argc, char* argv[]){

    // first thing
    //Check_SDL_Version();
    printf("total system memory: %i\n", max_address);

    // initialized M6502 variable
    struct M6502 computer;
    M6502_init(&computer);
    printf("size of virtual computer is: %lli bytes\n", sizeof(struct M6502));

    // load program into memory - array of uchar8_t
    uchar8_t* program = load_program(argc, argv);
    print_program_info(argc, argv);
    ushort16_t program_size = get_program_size(argv);
    printf("program size: %i bytes\n", program_size);
    M6502_store_program(&computer, program, program_size);
    free(program);

    execute_instruction(&computer, program_size);
    
    // stack stuff
    M6502_stack_init(&computer);

    // push and pop to stack
    M6502_stack_push(&computer, 0x101);
    M6502_stack_pop(&computer, 0x102);

    return EXIT_SUCCESS;
}