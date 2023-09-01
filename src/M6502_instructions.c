#include "M6502_instructions.h"
#include "M6502_memory.h"
#include "config.h"
#include <stdio.h>


// executes all program instructions
void execute_instruction(struct M6502* computer, ushort16_t program_size){
    // for loop takes program size and subtracts as instructions happen
    for(ushort16_t i = 0; i < (program_size - (instruction_byte_count)); i++){
        uchar8_t opcode = instruction_fetch(computer);
        analyze_opcode(computer, opcode);
        program_counter += 1;
        // Note for this function - it goes to analyze_opcode and PC is +1 for instruction call
        // If the instruction needs to  return byte, it will already be at that address
        // If the instruction needs to return word, the PC is +1 from M6502_get_word()
        // When it returns from instruction the PC is +1 to prepare for next opcode
        // Implied and Accumulator will need to decrement PC by -1 as they are only 1 byte in total
    }
}

// sets provided status register flag
void set_flag(struct M6502* computer, uchar8_t FLAG){

    // Another way could be to shift 1 << 6 - 0000 0001 -> 0100 0000
    switch(FLAG)
    {
        case CARRY:
            status_register |= flag_carry_bit;
            break;
        case ZERO:
            status_register |= flag_zero_bit;
            break;
        case INTERRUPT:
            status_register |= flag_interrupt_bit;
            break;
        case DECIMAL:
            status_register |= flag_decimal_bit;
            break;
        case BREAK:
            status_register |= flag_break_bit;
            break;
        case IGNORED:

            break;
        case OVERFLOW:
            status_register |= flag_overflow_bit;
            break;
        case NEGATIVE:
            status_register |= flag_negative_bit; // OR operation is inclusive
            break;
        default:
            puts("Error: not a valid flag");
        break;
    }
}

// check if provided flag needs to be set, this also sets that flag
void check_flag(struct M6502* computer, uchar8_t FLAG, uchar8_t test_against){

    switch(FLAG)
    {
        case CARRY:
            break;
        case ZERO:
            if(accumulator == 0)
                puts("Set Zero flag!");
            break;
        case INTERRUPT:
            break;
        case DECIMAL:
            break;
        case BREAK:
            break;
        case IGNORED:
            break;
        case OVERFLOW:

            break;
        case NEGATIVE:
            if(flag_negative_bit & test_against){
                puts("Sign is negative!");
                set_flag(computer, NEGATIVE);
            }
            break;
        default:
            puts("Error: not a valid flag");
        break;
    }
}

// this may actually need the address input after instruction performed
void check_page(struct M6502* computer, ushort16_t input_address, uchar8_t register_n){
    
    ushort16_t input_address_offset = input_address + register_n;

    float old_page = input_address/256; // this naturally truncates 
    float new_page = input_address_offset/256;
    (old_page != new_page) ? cycle_push(1) : cycle_push(0); // if new page != old page +1 cycle
    //(old_page != new_page) ? puts("cyc: 1") :puts("cyc: 0"); // if new page != old page +1 cycle
}


void ADC(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case IMMEDIATE: // 0x69
            
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x65
            
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x75
            
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x6D
            
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x7D

            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0x79

            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0x61
            
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x71
            
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
            
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x25
            
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x35
            
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x2D

            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x3D

            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0x39

            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0x21
            
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x31
            
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
            
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x16
            
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x0E

            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x1E

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
        
        cycle_push(2); // +1 if branch succeeds, +2 if to a new page
}

void BRK(struct M6502* computer)
{
    cycle_push(7); // 0x00
}

void BCS(struct M6502* computer)
{
    
    cycle_push(2); // 0xB0 +1 if branch succeeds, +2 if to a new page
}

void BEQ(struct M6502* computer)
{
    
    cycle_push(2); // 0xF0 +1 if branch succeeds, +2 if to a new page
}

void BIT(struct M6502* computer, uchar8_t mode)
{
    switch(mode)
    {
        case ZERO_PAGE: // 0x24
            
            cycle_push(3);
            break;
        case ABSOLUTE: // 0x2C
            
            cycle_push(4);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void BMI(struct M6502* computer)
{
    
    cycle_push(2); // 0x30 +1 if branch succeeds, +2 if to a new page
}

void BNE(struct M6502* computer)
{
    
    cycle_push(2); // 0xD0 +1 if branch succeeds, +2 if to a new page
}

void BPL(struct M6502* computer)
{
    
    cycle_push(2); // 0x10 +1 if branch succeeds, +2 if to a new page
}

void BVC(struct M6502* computer)
{
    
    cycle_push(2); // 0x50 +1 if branch succeeds, +2 if to a new page
}

void BVS(struct M6502* computer)
{
    
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
            
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xC5
            
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0xD5
            
            cycle_push(4);
            break;
        case ABSOLUTE: // 0xCD
            
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0xDD
            

            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0xD9
            

            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0xC1
            
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0xD1
            
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
            
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xE4
            
            cycle_push(3);
            break;
        case ABSOLUTE: // 0xEC
            
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
            
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xC4
            
            cycle_push(3);
            break;
        case ABSOLUTE: // 0xCC

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
            
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0xD6
            
            cycle_push(6);
            break;
        case ABSOLUTE: // 0xCE

            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0xDE
            
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
            
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x45
            
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x55
            
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x4D

            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x5D

            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0x59
            

            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0x41
            
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x51
            
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
            
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0xF6
            
            cycle_push(6);
            break;
        case ABSOLUTE: // 0xEE

            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0xFE

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

            ushort16_t location = M6502_get_word(computer, program_counter, increment_true);
            program_counter = location;
            cycle_push(3);
            break;
        case INDIRECT: // 0x6C
            
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
        case IMMEDIATE: { // 0xA9
            // load immeadiate value at location
            accumulator = memory_address[program_counter];
            printf("Accumulator: %02X\n", accumulator);
            check_flag(computer, NEGATIVE, accumulator);
            check_flag(computer, ZERO, accumulator);
            cycle_push(2);
            break;
        }
        case ZERO_PAGE: { // 0xA5
            uchar8_t input_address = memory_address[program_counter];
            // load whats at the input address
            accumulator = memory_address[input_address];
            printf("Accumulator: %02X\n", accumulator);
            check_flag(computer, NEGATIVE, accumulator);
            check_flag(computer, ZERO, accumulator);
            cycle_push(3);
            break;
        }
        case ZERO_PAGE_X:{ // 0xB5
            uchar8_t input_address = memory_address[program_counter];
            accumulator = memory_address[(input_address + x_register)];
            printf("Accumulator: %i\n", accumulator);
            check_flag(computer, NEGATIVE, accumulator);
            check_flag(computer, ZERO, accumulator);
            cycle_push(4);
            break;
        }
        case ABSOLUTE: // 0xAD
            accumulator = memory_address[M6502_get_word(computer, program_counter, increment_true)];
            printf("Accumulator: %i\n", accumulator);
            check_flag(computer, NEGATIVE, accumulator);
            check_flag(computer, ZERO, accumulator);
            cycle_push(4);
            break;

        case ABSOLUTE_X:{ // 0xBD
            // get 16 bit address at current PC and offset by x register, store result in accumulator
            ushort16_t input_address = M6502_get_word(computer, program_counter, increment_true);
            accumulator = memory_address[input_address + x_register];
            printf("Accumulator: %i\n", accumulator);
            check_flag(computer, NEGATIVE, accumulator);
            check_flag(computer, ZERO, accumulator);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, x_register); // will cycle if page crossed
            break;
        }
        case ABSOLUTE_Y:{ // 0xB9
            ushort16_t input_address = M6502_get_word(computer, program_counter, increment_true);
            accumulator = memory_address[input_address + y_register];
            printf("Accumulator: %i\n", accumulator);
            check_flag(computer, NEGATIVE, accumulator);
            check_flag(computer, ZERO, accumulator);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, y_register); // will cycle if page crossed
            break;
        }
        case INDIRECT_X:{ // 0xA1
            // grab byte at current location and use as an address - 0x00
            uchar8_t input_byte = M6502_get_byte(computer, program_counter);
            ushort16_t indirect_byte_offset = input_byte + x_register;
            // stores contents in the accumulator
            accumulator = memory_address[indirect_byte_offset];
            printf("Accumulator: %02X\n", accumulator);
            check_flag(computer, NEGATIVE, accumulator);
            check_flag(computer, ZERO, accumulator);
            cycle_push(6);
            break;
        }
        case INDIRECT_Y:{ // 0xB1
            // grab byte at current location and use as an address - 0x00
            uchar8_t input_byte = M6502_get_byte(computer, program_counter);
            // get 16 bit address from the input byte + its next address - 0x00 0x01
            /* important to note here increment needs to be false because the instruction input
            is only a zero page address, the indirect address is abstracted as a word */
            ushort16_t indirect_address = M6502_get_word(computer, input_byte, increment_false);
            // offset the address by + y register
            ushort16_t indirect_address_offset = indirect_address + y_register;
            // store into accumlator the value at the final indirect address
            accumulator = memory_address[indirect_address_offset];
            printf("Accumulator: %02X\n", accumulator);
            check_flag(computer, NEGATIVE, accumulator);
            check_flag(computer, ZERO, accumulator);
            cycle_push(5); // +1 if page crossed
            check_page(computer, indirect_address, y_register); // will cycle if page crossed
            break;
        }
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

            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xA6

            cycle_push(3);
            break;
        case ZERO_PAGE_Y: // 0xB6

            cycle_push(4);
            break;
        case ABSOLUTE: // 0xAE

            cycle_push(4);
            break;
        case ABSOLUTE_Y: // 0xBE

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

            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xA4

            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0xB4

            cycle_push(4);
            break;
        case ABSOLUTE: // 0xAC

            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0xBC

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
            
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x56
            
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x4E
            
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x5E
            
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
            
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x05
            
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x15
            
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x0D
            
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x1D
            
            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0x19
            
            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0x01
            
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x11
            
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
            
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x36
            
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x2E
            
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x3E
            
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
            
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x76
            
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x6E
            
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x7E
            
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
            
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xE5
            
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0xF5
            
            cycle_push(4);
            break;
        case ABSOLUTE: // 0xED
            
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0xFD
            
            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0xF9
            
            cycle_push(4); // +1 if page crossed
            break;
        case INDIRECT_X: // 0xE1
            
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0xF1
            
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
            
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x95
            
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x8D
            
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x9D

            M6502_get_word(computer, program_counter, increment_true);
            cycle_push(5);
            break;
        case ABSOLUTE_Y: // 0x99
            
            cycle_push(5);
            break;
        case INDIRECT_X: // 0x81
            
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x91
            
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
            
            cycle_push(3);
            break;
        case ZERO_PAGE_Y: // 0x96
            
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x8E
            
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
            
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x94
            
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x8C
            
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

void TXS(struct M6502* computer){
    cycle_push(2); // 0x9A
}

void TYA(struct M6502* computer)
{
    cycle_push(2); // 0x98
}
