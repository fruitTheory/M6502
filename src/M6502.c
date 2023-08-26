#include "M6502.h"
#include <memory.h>
#include <assert.h>
#include "config.h"
#include "M6502_instructions.h"
#include <stdio.h>

// global flag var - 255 | 1111 1111
uchar8_t flag_bit = 0xFF;

// initialize the processor
void M6502_init(struct M6502* computer){
    // set any memory to 0
    memset(computer, 0, sizeof(&computer));

    printf("stack first addy: %i\n", computer->stack.stack_address[0]);
    computer->stack.stack_address[0] = 2;
    printf("size of stack array: %lli\n", sizeof(computer->stack.stack_address));

    memset(computer->stack.stack_address, 1, sizeof(computer->stack.stack_address));
    memcpy(&computer->memory.address[stack_start], &computer->stack.stack_address, sizeof(computer->stack.stack_address));

    printf("stack after copy: %i\n", computer->memory.address[stack_start-1]);
    printf("stack after copy: %i\n", computer->memory.address[stack_start]);
    printf("stack after copy: %i\n", computer->memory.address[stack_start+1]);

    //M6502_stack_init(computer);
}

// void analyze_opcode(uchar8_t opcode){
//     opcode
// }

// returns an opcode
uchar8_t instruction_fetch(struct M6502* computer, int index){
    printf("PC: %04X\n", computer->registers.PC);
    uchar8_t opcode = computer->memory.address[computer->registers.PC];
    printf("opcode: %02X\n", opcode);
    return opcode;
}

// determines which opcode was returned
void analyze_opcode(struct M6502* computer, uchar8_t opcode, int index){

    switch (opcode)
        // if instruction has addressing modes pass it a mode
    {
        // ADC - Add Memory to Accumulator with Carry
        case ADC_IMMEDIATE_D: 
            ADC(computer, IMMEDIATE);
            break;
        case ADC_ZERO_PAGE_D: 
            ADC(computer, ZERO_PAGE);
            break;
        case ADC_ZERO_PAGE_X_D: 
            ADC(computer, ZERO_PAGE_X);
            break;
        case ADC_ABSOLUTE_D: 
            ADC(computer, ABSOLUTE);
            break;
        case ADC_ABSOLUTE_X_D: 
            ADC(computer, ABSOLUTE_X);
            break;
        case ADC_ABSOLUTE_Y_D: 
            ADC(computer, ABSOLUTE_Y);
            break;
        case ADC_INDIRECT_X_D: 
            ADC(computer, INDIRECT_X);
            break;
        case ADC_INDIRECT_Y_D: 
            ADC(computer, INDIRECT_Y);
            break;

        // LDA - Load Accumulator
        case LDA_IMMEDIATE_D:
            //LDA_Immeadiate(computer);
            
            LDA(computer, IMMEDIATE, index);
            break;

        case LDA_ABSOLUTE_D:
            LDA(computer, ABSOLUTE, index);
            break;
        default:
            puts("case not found");
            break;
    }
}


