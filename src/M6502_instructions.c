#include "M6502_instructions.h"
#include "config.h"
#include <stdio.h>


void BRK(){
    puts("Im implicit");
    puts("Do a thing");
} // address the define issue

void ADC(struct M6502* M6502, ushort16_t user_value, uchar8_t mode)
{
    switch(mode)
    {
        case ACCUMULATOR:
            puts("Do Cmd");
            break;
        case IMMEDIATE: 
            puts("Do Cmd");
            break;
        case ZERO_PAGE:
            puts("Do Cmd");
            break;
        case ZERO_PAGE_X:
            puts("Do Cmd");
            break;
        case ZERO_PAGE_Y:
            puts("Do Cmd");
            break;
        case ABSOLUTE:
            puts("Do Cmd");
            break;
        case ABSOLUTE_X:
            puts("Do Cmd");
            break;
        case ABSOLUTE_Y:
            puts("Do Cmd");
            break;
        case INDIRECT:
            puts("Do Cmd");
            break;
        case INDIRECT_X:
            puts("Do Cmd");
            break;
        case INDIRECT_Y:
            puts("Do Cmd");
            break;
        case RELATIVE:
            puts("Do Cmd");
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
    // Cmd example
    M6502->registers.AC += M6502->memory.address[user_value];
}


/*
LDA(mosM6502, int user_value){
    if immeadiate
    mosM6502.registers.AC = user_value(19);

    if LDA zeropage(0xA5)
    mosM6502.registers.AC = user_value(0x00FF);
    Example: LDA $50 loads the value at memory address 0x0050.

    if LDA absolute(0xAD)
    mosM6502.registers.AC = user_value(0xFFFF);
}
ASL - shifts one bit left effectively doubling a number
*/

// Old way:
// void LDA_Immeadiate(struct M6502* M6502, uchar8_t user_value){
//     M6502->registers.AC = user_value;
// }

// void LDX_Zero_Page(struct M6502* M6502, uchar8_t user_value){
//     M6502->registers.AC = user_value;
// }

// void LDX_Zero_Page_X(struct M6502* M6502, uchar8_t user_value){
//     M6502->registers.AC = user_value + M6502->registers.X;
// }

// void LDA_Absolute(struct M6502* M6502, ushort16_t user_value);