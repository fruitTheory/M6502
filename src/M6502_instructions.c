#include "M6502_instructions.h"
#include "config.h"
#include <stdio.h>
#include "M6502_memory.h"

// A container for bytes to help know when program instruction ends -
// It is the total amount of bytes needed by executed instructions
static ushort16_t instruction_byte_count;

// increment program counter by + 1, Used by 2 byte instructions
static void Byte_Increment(struct M6502* computer){
    computer->registers.PC += 1;
    instruction_byte_count += 1;
}
// increment program counter by + 2, Used by 3 byte instructions
static void Word_Increment(struct M6502* computer){
    computer->registers.PC += 2;
    instruction_byte_count += 2;
}

// executes all program instructions
void execute_instruction(struct M6502* computer, ushort16_t program_size){

    computer->memory.address[0x0262] = 69;
    for(ushort16_t i = 0; i < (program_size - instruction_byte_count); i++){
        //printf("global: %i\n",instruction_byte_count);
        uchar8_t opcode = instruction_fetch(computer, i);
        analyze_opcode(computer, opcode, i);
        // natural program counter
        computer->registers.PC += 1;
    }
}

void LDA(struct M6502* computer, uchar8_t mode, int index){
        switch(mode)
    {
        case IMMEDIATE:
            Byte_Increment(computer);
            computer->registers.AC = computer->memory.address[computer->registers.PC];
            printf("LDA stored in AC: %02X\n", computer->memory.address[computer->registers.PC]);
            printf("Accumulator: %02X\n", computer->registers.AC);
            
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
        case ABSOLUTE: // address 0x0262
            printf("PC from func: %04X\n", computer->registers.PC);
            Word_Increment(computer);
            printf("PC from func: %04X\n", computer->registers.PC);
            printf("Accumulator: %02X\n", computer->registers.AC);
            computer->registers.AC = computer->memory.address[M6502_memory_get_word(computer, computer->registers.PC)];
            printf("Accumulator: %i\n", computer->registers.AC);
            
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
}


// void LDA_Immeadiate(struct M6502* computer){
    
//     //uchar8_t accumulator = computer->registers.AC;
//     printf("Accumulator: %02X\n", Accumulator(computer));
//     computer->registers.AC = computer->memory.address[0x201];
//     printf("LDA stored in AC: %02X\n", computer->memory.address[0x201]);
//     printf("Accumulator: %02X\n", Accumulator(computer));
// }

void ADC(struct M6502* computer, uchar8_t mode)
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