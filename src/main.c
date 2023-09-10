#include "config.h"
#include "M6502.h"
#include "load_binary.h"
#include <stdio.h>
#include <stdlib.h>
#include "M6502_stack.h"

int main(int argc, char* argv[]){

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

    return EXIT_SUCCESS;
}