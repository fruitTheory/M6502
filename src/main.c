#include <stdio.h>
#include "config.h"
#include "check_SDL_version.h"
#include "M6502.h"
#include "M6502_stack.h"
#include "load_binary.h"
#include <stdlib.h>
#include "memory.h"
#include "M6502_instructions.h"


int main(int argc, char* argv[]){

    // first thing
    //Check_SDL_Version();
    printf("total system memory: %i\n", max_memory);
    flag_bit -= 5;
    printf("flag bit: %i\n", flag_bit);
    // initialized M6502 variable
    struct M6502 computer;
    M6502_init(&computer);
    printf("size of computer is: %lli bytes\n", sizeof(struct M6502));
    
    // load program into memory - array of uchar8_t
    uchar8_t* program = load_program(argc, argv);
    print_program_info(argc, argv);
    ushort16_t program_size = get_program_size(argv);
    printf("program size: %i bytes\n", program_size);
    M6502_store_program(&computer, program, program_size);
    free(program);

    execute_instruction(&computer, program_size);
    
    // computer.memory.address[0x0262] = 69;
    // for(ushort16_t i = 0; i < (program_size - global_byte_count); i++){
    //     printf("global: %i\n",global_byte_count);
    //     uchar8_t opcode = instruction_fetch(&computer, i);
    //     analyze_opcode(&computer, opcode, i);
    //     computer.registers.PC += 1;
    // }

    // uchar8_t opcode = instruction_fetch(&computer);
    // analyze_opcode(&computer, opcode);

    // // stack stuff
    // M6502_stack_init(&computer);
    // printf("stack unitialized: %i\n", computer.registers.SP);
    // computer.registers.SP = 5;
    // printf("stack initialized: %i\n", computer.registers.SP);

    // // push and pop to stack
    // M6502_stack_push(&computer, 0x100);
    // M6502_stack_pop(&computer, 0x100);

    return EXIT_SUCCESS;
}