#include "M6502.h"
#include <memory.h>
#include <assert.h>
#include "config.h"
#include "M6502_instructions.h"


// global flag var - 255 | 1111 1111
uchar8_t flag_bit = 0xFF;

// initialize the processor
void M6502_init(struct M6502* M6502){
    memset(M6502, 0, sizeof(&M6502));
    M6502_stack_init(M6502);
}

void execute_instruction(struct M6502* M6502, uchar8_t opcode){

    uchar8_t user_value = 1;
    switch (opcode)
        // if instruction has addressing modes pass it a mode
    {
        // ADC - Add Memory to Accumulator with Carry
        // case ADC_ACCUMULATOR_D: 
        //     ADC(M6502, user_value, IMMEDIATE);
        //     break;
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


