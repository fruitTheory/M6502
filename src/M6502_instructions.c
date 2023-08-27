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

    // for loop takes program size and subtracts as instructions happen
    for(ushort16_t i = 0; i < (program_size - instruction_byte_count); i++){
        //printf("global: %i\n",instruction_byte_count);
        uchar8_t opcode = instruction_fetch(computer);
        analyze_opcode(computer, opcode);
        // natural program counter increment for instruction call
        computer->registers.PC += 1;
    }
}
void CLC(){
    puts("CLC -");
}

void LDA(struct M6502* computer, uchar8_t mode){
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
        case ABSOLUTE:
            printf("Accumulator: %02X\n", computer->registers.AC);
            printf("MEMORY ADDRESS : %04X\n",M6502_memory_get_word(computer, computer->registers.PC));

            computer->memory.address[0x0262] = 69;
            computer->registers.AC = computer->memory.address[M6502_memory_get_word(computer, computer->registers.PC)];
            Word_Increment(computer);

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
ASL - shifts one bit left effectively doubling a number
*/
