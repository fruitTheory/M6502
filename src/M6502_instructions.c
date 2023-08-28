#include "M6502_instructions.h"
#include "config.h"
#include <stdio.h>
#include "M6502_memory.h"

static ulong64_t cycles;
// A container for bytes to help know when program instruction ends -
// It is the total amount of bytes needed by executed instructions
static ushort16_t instruction_byte_count;

// increment program counter by + 1, Used by 2 byte instructions
static inline void Byte_Increment(struct M6502* computer){
    computer->registers.PC += 1;
    instruction_byte_count += 1;
}
// increment after a 2 byte operation
static inline void Word_Increment(struct M6502* computer){
    computer->registers.PC += 2;
    instruction_byte_count += 2;
}
// increment cycle count
void cycle_push(uchar8_t cycle){
    cycles += cycle;
}
// return cycle count
ulong64_t cycle_pop(){
    printf("cycles: %li\n", cycles);
    return cycles;
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

void ADC(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case IMMEDIATE: // 0x69
            Byte_Increment(computer);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x65
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x75
            Byte_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x6D
            Word_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x7D
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0x79
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0x61
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x71
            Byte_Increment(computer);
            cycle_push(5); // +1 if page crossed
            break;
        default:
            puts("Error: Please specify addressing mode for ADC");
            break;
    }
}


void AND(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case IMMEDIATE: // 0x29
            Byte_Increment(computer);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x25
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x35
            Byte_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x2D
            Word_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x3D
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0x39
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0x21
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x31
            Byte_Increment(computer);
            cycle_push(5); // +1 if page crossed
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void ASL(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ACCUMULATOR: // 0x0A
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x06
            Byte_Increment(computer);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x16
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x0E
            Word_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x1E
            Word_Increment(computer);
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// 0x90 - Relative
void BCC(struct M6502* computer)
{
        Byte_Increment(computer);
        cycle_push(2); // +1 if branch succeeds, +2 if to a new page
}

void BRK(struct M6502* computer)
{
    cycle_push(7); // 0x00
}

void BCS(struct M6502* computer)
{
    Byte_Increment(computer);
    cycle_push(2); // 0xB0 +1 if branch succeeds, +2 if to a new page
}

void BEQ(struct M6502* computer)
{
    Byte_Increment(computer);
    cycle_push(2); // 0xF0 +1 if branch succeeds, +2 if to a new page
}

void BIT(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ZERO_PAGE: // 0x24
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ABSOLUTE: // 0x2C
            Word_Increment(computer);
            cycle_push(4);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void BMI(struct M6502* computer)
{
    Byte_Increment(computer);
    cycle_push(2); // 0x30 +1 if branch succeeds, +2 if to a new page
}

void BNE(struct M6502* computer)
{
    Byte_Increment(computer);
    cycle_push(2); // 0xD0 +1 if branch succeeds, +2 if to a new page
}

void BPL(struct M6502* computer)
{
    Byte_Increment(computer);
    cycle_push(2); // 0x10 +1 if branch succeeds, +2 if to a new page
}

void BVC(struct M6502* computer)
{
    Byte_Increment(computer);
    cycle_push(2); // 0x50 +1 if branch succeeds, +2 if to a new page
}

void BVS(struct M6502* computer)
{
    Byte_Increment(computer);
    cycle_push(2); // 0x70 +1 if branch succeeds, +2 if to a new page
}

void CLC(struct M6502* computer)
{
    cycle_push(2); // 0x18
}

void CLD(struct M6502* computer)
{
    cycle_push(2); // 0xD8
}

void CLI(struct M6502* computer)
{
    cycle_push(2); // 0x58
}

void CLV(struct M6502* computer)
{
    cycle_push(2); // 0xB8
}

void CMP(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case IMMEDIATE: // 0xC9
            Byte_Increment(computer);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xC5
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0xD5
            Byte_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0xCD
            Word_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0xDD
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0xD9
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0xC1
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0xD1
            Byte_Increment(computer);
            cycle_push(5); // +1 if page crossed
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void CPX(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case IMMEDIATE: // 0xE0
            Byte_Increment(computer);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xE4
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ABSOLUTE: // 0xEC
            Word_Increment(computer);
            cycle_push(4);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void CPY(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case IMMEDIATE: // 0xC0
            Byte_Increment(computer);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xC4
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ABSOLUTE: // 0xCC
            Word_Increment(computer);
            cycle_push(4);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void DEC(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ZERO_PAGE: // 0xC6
            Byte_Increment(computer);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0xD6
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE: // 0xCE
            Word_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0xDE
            Word_Increment(computer);
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void DEX(struct M6502* computer)
{
    cycle_push(2); // 0xCA
}

void DEY(struct M6502* computer)
{
    cycle_push(2); // 0x88
}

void EOR(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case IMMEDIATE: // 0x49
            Byte_Increment(computer);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x45
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x55
            Byte_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x4D
            Word_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x5D
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0x59
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0x41
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x51
            Byte_Increment(computer);
            cycle_push(5); // +1 if page crossed
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void INC(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ZERO_PAGE: // 0xE6
            Byte_Increment(computer);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0xF6
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE: // 0xEE
            Word_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0xFE
            Word_Increment(computer);
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void INX(struct M6502* computer)
{
    cycle_push(2); // 0xE8
}

void INY(struct M6502* computer)
{
    cycle_push(2); // 0xC8
}

void JMP(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ABSOLUTE: // 0x4C
            Word_Increment(computer);
            cycle_push(3);
            break;
        case INDIRECT: // 0x6C
            Word_Increment(computer);
            cycle_push(5);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void JSR(struct M6502* computer, uchar8_t mode)
{
    if (mode == ABSOLUTE) // 0x20
    {
        Word_Increment(computer);
        cycle_push(6);
    }
    else
    {
        puts("Error: Please specify addressing mode");
    }
}

void LDA(struct M6502* computer, uchar8_t mode){

    switch(mode)
    {
        case IMMEDIATE: // 0xA9
            Byte_Increment(computer);
            computer->registers.AC = computer->memory.address[computer->registers.PC];
            printf("Accumulator: %02X\n", computer->registers.AC);
            //if(computer->registers.AC > 0x007F)
            cycle_push(2);
            cycle_pop();
            break;

        case ZERO_PAGE: // 0xA5
            Byte_Increment(computer);
            computer->registers.AC = computer->memory.address[computer->registers.PC];
            printf("Accumulator: %02X\n", computer->registers.AC);
            break;

        case ZERO_PAGE_X: // 0xB5
            computer->registers.X = 0x40;
            computer->memory.address[0x0090] = 69;
            Byte_Increment(computer);
 
            printf("X reg: %02X\n", computer->registers.X);
            uchar8_t result = computer->memory.address[computer->registers.PC];

            printf("result PC: %04X\n", (result+computer->registers.X));
            computer->registers.AC = computer->memory.address[(result+computer->registers.X)];
            printf("Accumulator: %i\n", computer->registers.AC);
            break;

        case ABSOLUTE: // 0xAD
            computer->memory.address[0x0262] = 69; // temp address and value
            // needs the current point counter and then get the bytes of the next two addresses
            computer->registers.AC = computer->memory.address[M6502_memory_get_word(computer, computer->registers.PC)];
            Word_Increment(computer);

            printf("Accumulator: %i\n", computer->registers.AC);

            break;

        case ABSOLUTE_X: // 0xBD
            computer->memory.address[0x0262] = 69; // temp address and value
            // needs the current point counter and then get the bytes of the next two addresses
            computer->registers.AC = computer->memory.address[M6502_memory_get_word(computer, computer->registers.PC+computer->registers.X)];
            Word_Increment(computer);

            printf("Accumulator: %i\n", computer->registers.AC);
            break;
        case ABSOLUTE_Y: // 0xBD
            puts("Do Cmd");
            break;

        case INDIRECT_X: // 0xA1
            Byte_Increment(computer);
            //printf("X reg: %02X\n", computer->registers.X);
            computer->registers.AC = computer->memory.address[computer->registers.PC + computer->registers.X];
            printf("Accumulator: %02X\n", computer->registers.AC);
            break;

        case INDIRECT_Y: // 0xB1
            Byte_Increment(computer);
            computer->registers.AC = computer->memory.address[computer->registers.PC+computer->registers.Y];
            printf("Accumulator: %02X\n", computer->registers.AC);
            break;

        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void LDX(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case IMMEDIATE: // 0xA2
            Byte_Increment(computer);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xA6
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ZERO_PAGE_Y: // 0xB6
            Byte_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0xAE
            Word_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE_Y: // 0xBE
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void LDY(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case IMMEDIATE: // 0xA0
            Byte_Increment(computer);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xA4
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0xB4
            Byte_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0xAC
            Word_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0xBC
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void LSR(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ACCUMULATOR: // 0x4A
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x46
            Byte_Increment(computer);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x56
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x4E
            Word_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x5E
            Word_Increment(computer);
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void NOP(struct M6502* computer)
{
    cycle_push(2); // 0xEA
}

void ORA(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case IMMEDIATE: // 0x09
            Byte_Increment(computer);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x05
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x15
            Byte_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x0D
            Word_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x1D
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0x19
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0x01
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x11
            Byte_Increment(computer);
            cycle_push(5); // +1 if page crossed
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void PHA(struct M6502* computer)
{
    cycle_push(3); // 0x48
}

void PHP(struct M6502* computer)
{
    cycle_push(3); // 0x08
}

void PLA(struct M6502* computer)
{
    cycle_push(4); // 0x68
}

void PLP(struct M6502* computer)
{
    cycle_push(4); // 0x28
}

void ROL(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ACCUMULATOR: // 0x2A
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x26
            Byte_Increment(computer);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x36
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x2E
            Word_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x3E
            Word_Increment(computer);
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void ROR(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ACCUMULATOR: // 0x6A
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x66
            Byte_Increment(computer);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x76
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x6E
            Word_Increment(computer);
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x7E
            Word_Increment(computer);
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void RTI(struct M6502* computer)
{
    cycle_push(6); // 0x40
}

void RTS(struct M6502* computer)
{
    cycle_push(6); // 0x60
}

void SBC(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case IMMEDIATE: // 0xE9
            Byte_Increment(computer);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xE5
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0xF5
            Byte_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0xED
            Word_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0xFD
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0xF9
            Word_Increment(computer);
            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0xE1
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0xF1
            Byte_Increment(computer);
            cycle_push(5); // +1 if page crossed
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void SEC(struct M6502* computer)
{
    cycle_push(2); // 0x38
}

void SED(struct M6502* computer)
{
    cycle_push(2); // 0xF8
}

void SEI(struct M6502* computer)
{
    cycle_push(2); // 0x78
}

void STA(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ZERO_PAGE: // 0x85
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x95
            Byte_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x8D
            Word_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x9D
            Word_Increment(computer);
            cycle_push(5);
            break;
        case ABSOLUTE_Y: // 0x99
            Word_Increment(computer);
            cycle_push(5);
            break;
        case INDIRECT_X: // 0x81
            Byte_Increment(computer);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x91
            Byte_Increment(computer);
            cycle_push(6);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void STX(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ZERO_PAGE: // 0x86
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ZERO_PAGE_Y: // 0x96
            Byte_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x8E
            Word_Increment(computer);
            cycle_push(4);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void STY(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ZERO_PAGE: // 0x84
            Byte_Increment(computer);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x94
            Byte_Increment(computer);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x8C
            Word_Increment(computer);
            cycle_push(4);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void TAX(struct M6502* computer)
{
    cycle_push(2); // 0xAA
}

void TAY(struct M6502* computer)
{
    cycle_push(2); // 0xA8
}

void TSX(struct M6502* computer)
{
    cycle_push(2); // 0xBA
}

void TXA(struct M6502* computer)
{
    cycle_push(2); // 0x8A
}

void TXS(struct M6502* computer)
{
    cycle_push(2); // 0x9A
}

void TYA(struct M6502* computer)
{
    cycle_push(2); // 0x98
}

// This concludes the 6502 instruction set functions
