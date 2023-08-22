#include "M6502.h"
#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include "config.h"
#include "M6502_instructions.h"

// initialize the processor
void M6502_init(struct M6502* M6502){
    memset(M6502, 0, sizeof(&M6502));
    M6502_stack_init(M6502);
}

// store the program into memory
void store_program(struct M6502* M6502, uchar8_t* file, size_t program_size){

    assert(stack_end+program_size < max_memory);

    // copy file into the program load location
    memcpy(&M6502->memory.address[program_initial_load], file, program_size);
    M6502->registers.PC = program_initial_load;

    // print all bytes of file
    for(int i = 0; i < (program_size); i++){
        printf("value at memory address %04X: %02X\n", M6502->registers.PC+i, M6502->memory.address[program_initial_load+i]);
    }
}

void execute_instruction(struct M6502* M6502, uchar8_t opcode){

    uchar8_t user_value = 1;
    switch (opcode)
        // if instruction has addressing modes pass it a mode
    {
        // ADC - Add Memory to Accumulator with Carry
        case ADC_IMMEDIATE_D: 
            ADC(M6502, user_value, IMMEDIATE);
            break;
        case ADC_ZERO_PAGE_D: 
            ADC(M6502, user_value, ZERO_PAGE);
            break;
        case ADC_ZERO_PAGE_X_D: 
            ADC(M6502, user_value, ZERO_PAGE_X);
            break;
        case ADC_ABSOLUTE_D: 
            ADC(M6502, user_value, ABSOLUTE);
            break;
        case ADC_ABSOLUTE_X_D: 
            ADC(M6502, user_value, ABSOLUTE_X);
            break;
        case ADC_ABSOLUTE_Y_D: 
            ADC(M6502, user_value, ABSOLUTE_Y);
            break;
        case ADC_INDIRECT_X_D: 
            ADC(M6502, user_value, INDIRECT_X);
            break;
        case ADC_INDIRECT_Y_D: 
            ADC(M6502, user_value, INDIRECT_Y);
            break;

        case BRK_D:
            break;
        default:
            break;
    }
}


