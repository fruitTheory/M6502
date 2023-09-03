#include "config.h"
#include "M6502.h"
#include "M6502_instructions.h"
#include "M6502_flags.h"
#include "M6502_memory.h"
#include <stdio.h>


// executes all program instructions
void execute_instruction(struct M6502* computer, ushort16_t program_size){
    // for loop takes program size and subtracts as instructions happen
    for(ushort16_t i = 0; i < (program_size - (instruction_byte_count)); i++){
        uchar8_t opcode = instruction_fetch(computer);
        analyze_opcode(computer, opcode);
        program_counter += 1;
        // Note for this function - it goes to analyze_opcode and PC is +1 for instruction call
        // If the instruction needs to return byte, it will already be at that address
        // If the instruction needs to return word, the PC is +1 from M6502_get_word()
        // When it returns from instruction the PC is +1 to prepare for next opcode
        // Implied and Accumulator will need to decrement PC by -1 as they are only 1 byte in total
    }
}

void ADC(struct M6502* computer, uchar8_t mode){

    // Add memory to accumulator with carry value
    // should really only be used after an instruction that causes overflow
    //uchar8_t result;
    switch(mode)
    {
        case IMMEDIATE: // 0x69
            accumulator = accumulator + memory_address[program_counter] + flag_carry_bit;
            printf("result: %02X\n", accumulator);
            check_flag_ZN(computer, accumulator);
            set_flag(computer, CARRY);
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


void AND(struct M6502* computer, uchar8_t mode){
    // logical AND performed on accumulator against memory
    // input address doesnt need to be ushort for zero page but keeping design general
    // view LDA for more detail on indirect addressing
    ushort16_t input_address;
    ushort16_t offset_address;
    ushort16_t word_address;
    switch(mode)
    {
        case IMMEDIATE: // 0x29
            accumulator = accumulator & memory_address[program_counter];
            check_flag_ZN(computer, accumulator);
            check_flag(computer, CARRY, accumulator);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x25
            input_address = memory_address[program_counter];
            accumulator = accumulator & memory_address[input_address];
            check_flag_ZN(computer, accumulator);
            check_flag(computer, CARRY, accumulator);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x35
            input_address = memory_address[program_counter];
            offset_address = input_address + x_register;
            accumulator = accumulator & memory_address[offset_address];
            check_flag_ZN(computer, accumulator);
            check_flag(computer, CARRY, accumulator);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x2D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            accumulator = accumulator & memory_address[input_address];
            check_flag_ZN(computer, accumulator);
            check_flag(computer, CARRY, accumulator);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x3D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            accumulator = accumulator & memory_address[offset_address];
            check_flag_ZN(computer, accumulator);
            check_flag(computer, CARRY, accumulator);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, x_register, 1);
            break;
        case ABSOLUTE_Y: // 0x39
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + y_register;
            accumulator = accumulator & memory_address[offset_address];
            check_flag_ZN(computer, accumulator);
            check_flag(computer, CARRY, accumulator);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, y_register, 1);
            break;
        case INDIRECT_X: // 0x21
            // this one just takes a byte as an address
            input_address = M6502_get_byte(computer, program_counter);
            offset_address = input_address + x_register;
            word_address = M6502_get_word(computer, offset_address, increment_false);
            // AND operation against whats at the word address
            accumulator = accumulator & memory_address[offset_address];
            check_flag_ZN(computer, accumulator);
            check_flag(computer, CARRY, accumulator);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x31
            input_address = M6502_get_byte(computer, program_counter);
            word_address = M6502_get_word(computer, input_address, increment_true);
            offset_address = word_address + y_register;
            // AND operation against whats at the offset address
            accumulator = accumulator & offset_address;
            check_flag_ZN(computer, accumulator);
            check_flag(computer, CARRY, accumulator);
            cycle_push(5); // +1 if page crossed
            check_page(computer, input_address, y_register, 1);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void ASL(struct M6502* computer, uchar8_t mode){
    // Arithmetic shift left
    // should probably test these work
    // * need to work on carry flag 
    ushort16_t input_address;
    ushort16_t offset_address;
    ushort16_t m_result;

    switch(mode)
    {
        case ACCUMULATOR: // 0x0A
            // note using flag negative bit just the check 7th bit
            //accumulator ;
            // shift accumulator 1 bit to the left
            accumulator <<= 1;
            check_flag_ZN(computer, accumulator);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x06
            // shift the value provided at current location 1 bit to the left 
            m_result = memory_address[program_counter] <<= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x16
            offset_address = memory_address[program_counter] + x_register;
            // shift the value provided at this location 1 bit to the left 
            m_result = memory_address[offset_address] <<= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x0E
            // provides a 16 bit address to work from
            input_address = M6502_get_word(computer, program_counter, increment_true);
            // shift the value provided at this location 1 bit to the left 
            m_result = memory_address[input_address] <<= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x1E
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            m_result = memory_address[offset_address] <<= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void BCC(struct M6502* computer){ // 0x90
    // Branch if carry clear
    // if carry bit not set to 1, do branch to PC
    if(!(status_register & flag_carry_bit)){
        //PC += relative_displacement;
        cycle_push(1);
    }
    cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

void BCS(struct M6502* computer){ // 0xB0
    // Branch if carry set
    // if carry bit is set to 1, do branch to PC
    if(status_register & flag_carry_bit){
        //PC += relative_displacement;
        cycle_push(1);
    }
    cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

void BEQ(struct M6502* computer){ // 0xF0
    
    // if zero flag is set to 1, do branch to PC
    if(status_register & flag_zero_bit){
        //PC += relative_displacement;
        cycle_push(1);
    }
    cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

void BIT(struct M6502* computer, uchar8_t mode){

    // Bit test, bitwise operation stuff tbd
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

void BMI(struct M6502* computer){ // 0x30
    // Branch if minus
    // if negative flag is set, do branch to PC
    if(status_register & flag_negative_bit){
        //PC += relative_displacement;
        cycle_push(1);
    }
    cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

void BNE(struct M6502* computer){ // 0xD0
    // Branch if not equal
    // if zero flag is clear, do branch to PC
    if(!(status_register & flag_zero_bit)){
        //PC += relative_displacement;
        cycle_push(1);
    }
    cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

void BPL(struct M6502* computer){ // 0x10
    // Branch if positive
    // if negative flag is clear, do branch to PC
    if(!(status_register & flag_negative_bit)){
        //PC += relative_displacement;
        cycle_push(1);
    }
    cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

void BRK(struct M6502* computer){ // 0x00
    M6502_stack_push(computer, program_counter);
    M6502_stack_push(computer, status_register);
    //program_counter = IRQ;
    set_flag(computer, BREAK);
    cycle_push(7);
}

void BVC(struct M6502* computer){ // 0x50
    // Branch if overflow clear
    // if overflow flag is clear, do branch to PC
    if(!(status_register & flag_overflow_bit)){
        //PC += relative_displacement;
        cycle_push(1);
    }
    cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

void BVS(struct M6502* computer){ // 0x70
    // Branch if overflow set
    // if overflow flag is set, do branch to PC
    if(status_register & flag_overflow_bit){
        //PC += relative_displacement;
        cycle_push(1);
    }
    cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

void CLC(struct M6502* computer){ // 0x18
    // Clear carry flag
    clear_flag(computer, CARRY);
    cycle_push(2);
}

void CLD(struct M6502* computer){ // 0xD8
    // Clear decimal mode
    clear_flag(computer, DECIMAL);
    cycle_push(2);
}

void CLI(struct M6502* computer){ // 0x58
    // Clear interrupt disable
    clear_flag(computer, INTERRUPT);
    cycle_push(2);
}

void CLV(struct M6502* computer){ // 0xB8
    // Clear overflow flag
    clear_flag(computer, OVERFLOW);
    cycle_push(2);
}

void CMP(struct M6502* computer, uchar8_t mode){
    
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

void CPX(struct M6502* computer, uchar8_t mode){
    
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

void CPY(struct M6502* computer, uchar8_t mode){
    
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

void DEC(struct M6502* computer, uchar8_t mode){
    
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

void DEX(struct M6502* computer){ // 0xCA
    x_register -= 1;
    check_flag_ZN(computer, x_register);
    cycle_push(2);
}

void DEY(struct M6502* computer){ // 0x88
    y_register -= 1;
    check_flag_ZN(computer, y_register);
    cycle_push(2);
}

void EOR(struct M6502* computer, uchar8_t mode){
    
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

void INC(struct M6502* computer, uchar8_t mode){
    
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

void INX(struct M6502* computer){ // 0xE8
    x_register += 1;
    check_flag_ZN(computer, x_register);
    cycle_push(2);
}

void INY(struct M6502* computer){ // 0xC8
    y_register += 1;
    check_flag_ZN(computer, y_register);
    cycle_push(2);
}

void JMP(struct M6502* computer, uchar8_t mode){
    
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

void JSR(struct M6502* computer, uchar8_t mode){

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
    // load memory into accumulator
    ushort16_t input_address;
    ushort16_t offset_address;
    ushort16_t word_address;
    switch(mode)
    {
        case IMMEDIATE: { // 0xA9
            // load immeadiate value at location
            accumulator = memory_address[program_counter];
            //printf("Accumulator: %02X\n", accumulator);
            check_flag_ZN(computer, accumulator);
            cycle_push(2);
            break;
        }
        case ZERO_PAGE: { // 0xA5
            input_address = memory_address[program_counter];
            // load whats at the input address
            accumulator = memory_address[input_address];
            //printf("Accumulator: %02X\n", accumulator);
            check_flag_ZN(computer, accumulator);
            cycle_push(3);
            break;
        }
        case ZERO_PAGE_X:{ // 0xB5
            input_address = memory_address[program_counter];
            accumulator = memory_address[(input_address + x_register)];
            //printf("Accumulator: %i\n", accumulator);
            check_flag_ZN(computer, accumulator);
            cycle_push(4);
            break;
        }
        case ABSOLUTE: // 0xAD
            accumulator = memory_address[M6502_get_word(computer, program_counter, increment_true)];
            //printf("Accumulator: %i\n", accumulator);
            check_flag_ZN(computer, accumulator);
            cycle_push(4);
            break;

        case ABSOLUTE_X:{ // 0xBD
            // get 16 bit address at current PC and offset by x register, store result in accumulator
            input_address = M6502_get_word(computer, program_counter, increment_true);
            accumulator = memory_address[input_address + x_register];
            //printf("Accumulator: %i\n", accumulator);
            check_flag_ZN(computer, accumulator);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, x_register, 1); // will cycle if page crossed
            break;
        }
        case ABSOLUTE_Y:{ // 0xB9
            input_address = M6502_get_word(computer, program_counter, increment_true);
            accumulator = memory_address[input_address + y_register];
            //printf("Accumulator: %i\n", accumulator);
            check_flag_ZN(computer, accumulator);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, y_register, 1); // will cycle if page crossed
            break;
        }
        case INDIRECT_X:{ // 0xA1
            // grab byte at current location and use as an address - 0x00
            input_address = M6502_get_byte(computer, program_counter);
            // offset it by x register
            offset_address = input_address + x_register;
            // abstractly or 'indirectly' grabbing a word sized address at the offset address
            word_address = M6502_get_word(computer, offset_address, increment_false);
            // stores contents of the word address in the accumulator
            accumulator = memory_address[word_address];
            //printf("Accumulator: %02X\n", accumulator);
            check_flag_ZN(computer, accumulator);
            cycle_push(6);
            break;
        }
        case INDIRECT_Y:{ // 0xB1
            // grab byte at current location and use as an address - 0x00
            input_address = M6502_get_byte(computer, program_counter);
            // get a word address at the input address + 1
            ushort16_t word_address = M6502_get_word(computer, input_address, increment_false);
            // offset the address by + y register
            ushort16_t offset_address = word_address + y_register;
            // store into accumlator the value at the final offset address
            accumulator = memory_address[offset_address];
            //printf("Accumulator: %02X\n", accumulator);
            check_flag_ZN(computer, accumulator);
            cycle_push(5); // +1 if page crossed
            check_page(computer, offset_address, y_register, 1); // will cycle if page crossed
            break;
        }
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void LDX(struct M6502* computer, uchar8_t mode){
    
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

void NOP(struct M6502* computer){ // 0xEA
    // no operation
    cycle_push(2);
}

void ORA(struct M6502* computer, uchar8_t mode){
    
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

void PHA(struct M6502* computer){ // 0x48
    M6502_stack_push(computer, accumulator);
    cycle_push(3);
}

void PHP(struct M6502* computer){ // 0x08
    // note says something about break flag be and bit 5 set to 1
    M6502_stack_push(computer, status_register);
    cycle_push(3);
}

void PLA(struct M6502* computer){ // 0x68
    // Pulls value from the stack puts into the accumulator
    // Zero and negative flags are set
    // double check it means stack pop and value from stack pointer
    accumulator = M6502_stack_pop(computer);
    check_flag_ZN(computer, accumulator);
    cycle_push(4);
}

void PLP(struct M6502* computer){ // 0x28
    // Pulls a value from the stack into the SR, sets flags based on the value pulled
    // double check it means stack pop and value from stack pointer
    status_register = M6502_stack_pop(computer);
    // set flag works here because we are already dealing with status register
    set_flags_all(computer);
    cycle_push(4);
}

void ROL(struct M6502* computer, uchar8_t mode){
    
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

void RTI(struct M6502* computer){ // 0x40

    cycle_push(6);
}

void RTS(struct M6502* computer){ // 0x60

    cycle_push(6);
}

void SBC(struct M6502* computer, uchar8_t mode){
    
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

void SEC(struct M6502* computer){ // 0x38
    // set carry flag
    set_flag(computer, CARRY);
    cycle_push(2);
}

void SED(struct M6502* computer){ // 0xF8
    // set decimal flag
    set_flag(computer, DECIMAL);
    cycle_push(2);
}

void SEI(struct M6502* computer){ // 0x78
    // set interrupt flag
    set_flag(computer, INTERRUPT);
    cycle_push(2);
}

void STA(struct M6502* computer, uchar8_t mode){
    // Store Accumulator into memory
    ushort16_t input_address;
    switch(mode)
    {
        case ZERO_PAGE: // 0x85
            
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x95
            
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x8D
            input_address = x_register;
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x9D

            input_address= M6502_get_word(computer, program_counter, increment_true);
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

void STX(struct M6502* computer, uchar8_t mode){
    // Store X register into memory
    ushort16_t input_address;
    switch(mode)
    {
        case ZERO_PAGE: // 0x86
            // store x register to user defined zero page
            memory_address[program_counter] = x_register;
            cycle_push(3);
            break;
        case ZERO_PAGE_Y: // 0x96
            // store x regiseter to user defined zero page plus y offset
            input_address = memory_address[program_counter] + y_register;
            memory_address[input_address] = x_register;
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x8E
            // store y register at user define absolute
            input_address = M6502_get_word(computer, program_counter, increment_true);
            memory_address[input_address] = x_register;
            cycle_push(4);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void STY(struct M6502* computer, uchar8_t mode){
    // Store Y register into memory
    ushort16_t input_address;
    switch(mode)
    {
        case ZERO_PAGE: // 0x84
            // store y register to user defined zero page
            memory_address[program_counter] = y_register;
            cycle_push(3);
            break;
        case ZERO_PAGE_X:{ // 0x94
            // store y regiseter to user defined zero page plus x offset
            input_address = memory_address[program_counter] + x_register;
            memory_address[input_address] = y_register;
            cycle_push(4);
            break;
        }
        case ABSOLUTE:{ // 0x8C
            // store y register at user define absolute
            input_address = M6502_get_word(computer, program_counter, increment_true);
            memory_address[input_address] = y_register;
            cycle_push(4);
            break;
        }
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

void TAX(struct M6502* computer){ // 0xAA
    // transfer accumulator to x register
    x_register = accumulator;
    check_flag_ZN(computer, x_register);
    cycle_push(2);
}

void TAY(struct M6502* computer){ // 0xA8
    // transfer accumulator to y register
    y_register = accumulator;
    check_flag_ZN(computer, y_register);
    cycle_push(2);
}

void TSX(struct M6502* computer){ // 0xBA
    // transfer stack pointer to x register
    x_register = stack_pointer;
    check_flag_ZN(computer, x_register);
    cycle_push(2);
}

void TXA(struct M6502* computer){ // 0x8A
    // transfer x register to accumulator
    accumulator = x_register;
    check_flag_ZN(computer, accumulator);
    cycle_push(2);
}

void TXS(struct M6502* computer){ // 0x9A
    // transfer x register to stack pointer
    stack_pointer = x_register;
    cycle_push(2);
}

void TYA(struct M6502* computer){ // 0x98
    // transfer y register to accumulator
    accumulator = y_register;
    check_flag(computer, ZERO, accumulator);
    check_flag(computer, NEGATIVE, accumulator);
    cycle_push(2);
}
