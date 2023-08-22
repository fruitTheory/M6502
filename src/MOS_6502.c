#include "MOS_6502.h"
#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include "config.h"

// initialize the processor
void MOS_6502_init(struct MOS_6502* MOS_6502){
    memset(MOS_6502, 0, sizeof(&MOS_6502));
    MOS_6502_stack_init(MOS_6502);
}

// store the program into memory
void store_program(struct MOS_6502* MOS_6502, uchar8_t* file, size_t program_size){

    assert(stack_end+program_size < max_memory);

    // copy file into the program load location
    memcpy(&MOS_6502->memory.address[program_initial_load], file, program_size);
    MOS_6502->registers.PC = program_initial_load;

    // print all bytes of file
    for(int i = 0; i < (program_size); i++){
        printf("value at memory address %04X: %02X\n", MOS_6502->registers.PC+i, MOS_6502->memory.address[program_initial_load+i]);
    }
}

void execute_instruction(struct MOS_6502* MOS_6502, uchar8_t opcode){

    uchar8_t user_value = 1;
    switch (opcode)
        // if instruction has addressing modes pass it a mode
    {
        // ADC - Add Memory to Accumulator with Carry
        case ADC_IMMEDIATE_D: 
            ADC(MOS_6502, user_value, IMMEDIATE);
            break;
        case ADC_ZERO_PAGE_D: 
            ADC(MOS_6502, user_value, ZERO_PAGE);
            break;
        case ADC_ZERO_PAGE_X_D: 
            ADC(MOS_6502, user_value, ZERO_PAGE_X);
            break;
        case ADC_ABSOLUTE_D: 
            ADC(MOS_6502, user_value, ABSOLUTE);
            break;
        case ADC_ABSOLUTE_X_D: 
            ADC(MOS_6502, user_value, ABSOLUTE_X);
            break;
        case ADC_ABSOLUTE_Y_D: 
            ADC(MOS_6502, user_value, ABSOLUTE_Y);
            break;
        case ADC_INDIRECT_X_D: 
            ADC(MOS_6502, user_value, INDIRECT_X);
            break;
        case ADC_INDIRECT_Y_D: 
            ADC(MOS_6502, user_value, INDIRECT_Y);
            break;

        case BRK_D:
            break;
        default:
            break;
    }
}


