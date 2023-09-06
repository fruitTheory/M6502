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

// Add memory to accumulator with carry value
void ADC(struct M6502* computer, uchar8_t mode){
    
    ushort16_t input_address;
    ushort16_t offset_address;
    ushort16_t word_address;
    ushort16_t short_result;
    uchar8_t flag_bit;
    switch(mode)
    {
        case IMMEDIATE: // 0x69
            // returns if flag bit is set
            flag_bit = is_flag_set(CARRY, status_register);
            // have to hack this at a higher level since char doesnt notify out of bounds
            short_result = accumulator + memory_address[program_counter] + flag_bit;
            accumulator = accumulator + memory_address[program_counter] + flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x65
            input_address = memory_address[program_counter];
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator + memory_address[input_address] + flag_bit;
            accumulator = accumulator + memory_address[input_address] + flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x75
            input_address = memory_address[program_counter];
            offset_address = input_address + x_register;
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator + memory_address[offset_address] + flag_bit;
            accumulator = accumulator + memory_address[offset_address] + flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x6D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator + memory_address[input_address] + flag_bit;
            accumulator = accumulator + memory_address[input_address] + flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x7D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator + memory_address[offset_address] + flag_bit;
            accumulator = accumulator + memory_address[offset_address] + flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, x_register, 1);
            break;
        case ABSOLUTE_Y: // 0x79
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + y_register;
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator + memory_address[offset_address] + flag_bit;
            accumulator = accumulator + memory_address[offset_address] + flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, y_register, 1);
            break;
        case INDIRECT_X: // 0x61
            input_address = memory_address[program_counter];
            offset_address = input_address + x_register;
            flag_bit = is_flag_set(CARRY, status_register);
            word_address = M6502_get_word(computer, offset_address, increment_true);
            short_result = accumulator + memory_address[word_address] + flag_bit;
            accumulator = accumulator + memory_address[word_address] + flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, word_address);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x71
            input_address = M6502_get_byte(computer, program_counter);
            word_address = M6502_get_word(computer, input_address, increment_true);
            offset_address = word_address + y_register;
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator + memory_address[offset_address] + flag_bit;
            accumulator = accumulator + memory_address[offset_address] + flag_bit;
            
            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, offset_address);
            cycle_push(5); // +1 if page crossed
            check_page(computer, input_address, y_register, 1);
            break;
        default:
            puts("Error: Please specify addressing mode for ADC");
            break;
    }
}

// logical execlusive AND performed on accumulator against memory
void AND(struct M6502* computer, uchar8_t mode){
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
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x25
            input_address = memory_address[program_counter];
            accumulator = accumulator & memory_address[input_address];
            check_flag_ZN(computer, accumulator);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x35
            input_address = memory_address[program_counter];
            offset_address = input_address + x_register;
            accumulator = accumulator & memory_address[offset_address];
            check_flag_ZN(computer, accumulator);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x2D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            accumulator = accumulator & memory_address[input_address];
            check_flag_ZN(computer, accumulator);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x3D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            accumulator = accumulator & memory_address[offset_address];
            check_flag_ZN(computer, accumulator);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, x_register, 1);
            break;
        case ABSOLUTE_Y: // 0x39
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + y_register;
            accumulator = accumulator & memory_address[offset_address];
            check_flag_ZN(computer, accumulator);
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
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x31
            input_address = M6502_get_byte(computer, program_counter);
            word_address = M6502_get_word(computer, input_address, increment_true);
            offset_address = word_address + y_register;
            // AND operation against whats at the offset address
            accumulator = accumulator & offset_address;
            check_flag_ZN(computer, accumulator);
            cycle_push(5); // +1 if page crossed
            check_page(computer, input_address, y_register, 1);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Arithmetic shift left
void ASL(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    ushort16_t offset_address;
    ushort16_t m_result;

    switch(mode)
    {
        case ACCUMULATOR: // 0x0A
            // shift accumulator 1 bit to the left
            // ifs returns 0 or 1 based on 7th bit - based on result either set or clear carry flag
            is_flag_set(NEGATIVE, accumulator) ? set_flag(computer, CARRY):clear_flag(computer, CARRY);
            accumulator <<= 1;
            check_flag_ZN(computer, accumulator);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x06
            // shift the value provided at current location 1 bit to the left 
            m_result = memory_address[program_counter];
            // another note, this should be checked before shifting left 
            is_flag_set(NEGATIVE, m_result) ? set_flag(computer, CARRY):clear_flag(computer, CARRY);
            m_result <<= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x16
            offset_address = memory_address[program_counter] + x_register;
            // shift the value provided at this location 1 bit to the left 
            m_result = memory_address[offset_address];
            is_flag_set(NEGATIVE, m_result) ? set_flag(computer, CARRY):clear_flag(computer, CARRY);
            m_result <<= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x0E
            // provides a 16 bit address to work from
            input_address = M6502_get_word(computer, program_counter, increment_true);
            // shift the value provided at this location 1 bit to the left 
            m_result = memory_address[input_address];
            is_flag_set(NEGATIVE, m_result) ? set_flag(computer, CARRY):clear_flag(computer, CARRY);
            m_result <<= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x1E
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            m_result = memory_address[offset_address];
            is_flag_set(NEGATIVE, m_result) ? set_flag(computer, CARRY):clear_flag(computer, CARRY);
            m_result <<= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Branch if carry clear
void BCC(struct M6502* computer){ // 0x90
    uchar8_t current_address_value;
    char8_t signed_address_value;
    ushort16_t old_program_counter;

    old_program_counter = program_counter;
    current_address_value = memory_address[program_counter];

    // if value is over 127 based on bit 7, subtract the value from 127 to return a signed result
    if(is_flag_set(CARRY, current_address_value))signed_address_value = 127 - current_address_value;

    // if carry bit is not set to 1, do branch to PC - branching adds a signed value to PC -128 0-127+
    if(!is_flag_set(CARRY, status_register)){
        program_counter += signed_address_value;
        cycle_push(1); // +1 cycle if branch succeeds, +1 if to a new page

        // check if new program counter crossed a page in reference to old PC
        check_page(computer, old_program_counter, program_counter, 1);
    }

}

// Branch if carry set
void BCS(struct M6502* computer){ // 0xB0
    // if carry bit is set to 1, do branch to PC
    if(is_flag_set(CARRY, status_register)){
        //PC += relative_displacement;

    }
    //cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

// Branch if equal
void BEQ(struct M6502* computer){ // 0xF0
    // if zero flag is set to 1, do branch to PC
    if(is_flag_set(ZERO, status_register)){
        //PC += relative_displacement;

    }
    //cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

// Bit test, bitwise operation stuff tbd
void BIT(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    uchar8_t address_value;
    switch(mode)
    {
        case ZERO_PAGE: // 0x24
            // get the zero page address and the value there
            input_address = memory_address[program_counter];
            address_value = memory_address[input_address];

            (accumulator & address_value) == 0 ? set_flag(computer, ZERO):clear_flag(computer, ZERO);

            // Checking bit 6/7 of provided value, set overflow/negative flag based on the result
            is_flag_set(OVERFLOW, address_value) ? set_flag(computer, OVERFLOW):clear_flag(computer, OVERFLOW);
            is_flag_set(NEGATIVE, address_value) ? set_flag(computer, NEGATIVE):clear_flag(computer, NEGATIVE);

            cycle_push(3);
            break;
        case ABSOLUTE: // 0x2C
            // get the absolute address and the value there
            input_address = M6502_get_word(computer, program_counter, increment_true);
            address_value = memory_address[input_address];

            (accumulator & address_value) == 0 ? set_flag(computer, ZERO):clear_flag(computer, ZERO);

            is_flag_set(OVERFLOW, address_value) ? set_flag(computer, OVERFLOW):clear_flag(computer, OVERFLOW);
            is_flag_set(NEGATIVE, address_value) ? set_flag(computer, NEGATIVE):clear_flag(computer, NEGATIVE);

            cycle_push(4);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Branch if negative (minus)
void BMI(struct M6502* computer){ // 0x30
    // if negative flag is set, do branch to PC
    if(is_flag_set(NEGATIVE, status_register)){
        //PC += relative_displacement;

    }
    //cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

// Branch if not equal
void BNE(struct M6502* computer){ // 0xD0
    // if zero flag is clear, do branch to PC
    if(!is_flag_set(ZERO, status_register)){
        //PC += relative_displacement;

    }
    //cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

// Branch if positive
void BPL(struct M6502* computer){ // 0x10
    // if negative flag is clear, do branch to PC
    if(!is_flag_set(NEGATIVE, status_register)){
        //PC += relative_displacement;

    }
    //cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

// Break interrupt
void BRK(struct M6502* computer){ // 0x00
    // push program coutner to stack
    uchar8_t extract_msb = program_counter>>8;
    uchar8_t extract_lsb = (char)program_counter;
    M6502_stack_push(computer, extract_msb); // push most significant byte
    M6502_stack_push(computer, extract_lsb); // push least significant byte

    // push status register to stack
    M6502_stack_push(computer, status_register);
    
    // set program coutner to request handler address
    program_counter = M6502_get_word(computer, IRQ, increment_false);
    printf("PC: %04X", program_counter);

    // break flag set to 1
    set_flag(computer, BREAK);
    cycle_push(7);
}

// Branch if overflow clear
void BVC(struct M6502* computer){ // 0x50
    // if overflow flag is clear, do branch to PC
    if(!is_flag_set(OVERFLOW, status_register)){
        //PC += relative_displacement;

    }
    //cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

// Branch if overflow set
void BVS(struct M6502* computer){ // 0x70
    // if overflow flag is set, do branch to PC
    if(is_flag_set(OVERFLOW, status_register)){
        //PC += relative_displacement;

    }
    //cycle_push(2); // +1 if branch succeeds, +2 if to a new page
    //check_page(computer, )
}

// Clear carry flag
void CLC(struct M6502* computer){ // 0x18
    clear_flag(computer, CARRY);
    cycle_push(2);
}

// Clear decimal mode
void CLD(struct M6502* computer){ // 0xD8
    clear_flag(computer, DECIMAL);
    cycle_push(2);
}

// Clear interrupt disable
void CLI(struct M6502* computer){ // 0x58
    clear_flag(computer, INTERRUPT);
    cycle_push(2);
}

// Clear overflow flag
void CLV(struct M6502* computer){ // 0xB8
    clear_flag(computer, OVERFLOW);
    cycle_push(2);
}

// Compare memory with accumulator
void CMP(struct M6502* computer, uchar8_t mode){

    // Substracts given value from register being compared, sets CZN flags
    ushort16_t result;
    ushort16_t input_address;
    ushort16_t offset_address;
    ushort16_t word_address;
    switch(mode)
    {
        case IMMEDIATE: // 0xC9
            result = accumulator - memory_address[program_counter];
            // Compare the accumulator against the value at memory address - set flags 
            if(accumulator >= memory_address[program_counter]) set_flag(computer, CARRY);
            if(accumulator == memory_address[program_counter]) set_flag(computer, ZERO);

            // Compare the result against negative flag
            check_flag_N(computer, result);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xC5
            input_address = memory_address[program_counter];
            result = accumulator - memory_address[input_address];

            if(accumulator >= memory_address[input_address]) set_flag(computer, CARRY);
            if(accumulator == memory_address[input_address]) set_flag(computer, ZERO);

            check_flag_N(computer, result);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0xD5
            input_address = memory_address[program_counter] + x_register;
            result = accumulator - memory_address[input_address];

            if(accumulator >= memory_address[input_address]) set_flag(computer, CARRY);
            if(accumulator == memory_address[input_address]) set_flag(computer, ZERO);

            check_flag_N(computer, result);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0xCD
            input_address = M6502_get_word(computer, program_counter, increment_true);
            result = accumulator - memory_address[input_address];

            if(accumulator >= memory_address[input_address]) set_flag(computer, CARRY);
            if(accumulator == memory_address[input_address]) set_flag(computer, ZERO);

            check_flag_N(computer, result);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0xDD
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            result = accumulator - memory_address[offset_address];

            if(accumulator >= memory_address[offset_address]) set_flag(computer, CARRY);
            if(accumulator == memory_address[offset_address]) set_flag(computer, ZERO);

            check_flag_N(computer, result);
            cycle_push(4); // +1 if page crossed
            break;
        case ABSOLUTE_Y: // 0xD9
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + y_register;
            result = accumulator - memory_address[offset_address];

            if(accumulator >= memory_address[offset_address]) set_flag(computer, CARRY);
            if(accumulator == memory_address[offset_address]) set_flag(computer, ZERO);

            check_flag_N(computer, result);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, y_register, 1);
            break;
        case INDIRECT_X: // 0xC1
            input_address = memory_address[program_counter];
            offset_address = input_address + x_register;
            word_address = M6502_get_word(computer, offset_address, increment_false);
            result = accumulator - memory_address[word_address];

            // Again compare the value at address against accumulator set flag based on this
            if(accumulator >= memory_address[word_address]) set_flag(computer, CARRY);
            if(accumulator == memory_address[word_address]) set_flag(computer, ZERO);

            check_flag_N(computer, result);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0xD1
            input_address = memory_address[program_counter];
            word_address = M6502_get_word(computer, input_address, increment_false);
            offset_address = word_address + y_register;
            result = accumulator - memory_address[offset_address];

            if(accumulator >= memory_address[offset_address]) set_flag(computer, CARRY);
            if(accumulator == memory_address[offset_address]) set_flag(computer, ZERO);

            check_flag_N(computer, result);
            cycle_push(5); // +1 if page crossed
            check_page(computer, word_address, y_register, 1);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Compare X register
void CPX(struct M6502* computer, uchar8_t mode){

    ushort16_t result;
    ushort16_t input_address;
    switch(mode)
    {
        case IMMEDIATE: // 0xE0
            result = x_register - memory_address[program_counter];
            if(x_register >= memory_address[program_counter]) set_flag(computer, CARRY);
            if(x_register == memory_address[program_counter]) set_flag(computer, ZERO);
            check_flag_N(computer, result);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xE4
            input_address = memory_address[program_counter];
            result = x_register - memory_address[input_address];

            if(x_register >= memory_address[input_address]) set_flag(computer, CARRY);
            if(x_register == memory_address[input_address]) set_flag(computer, ZERO);
            check_flag_N(computer, result);
            cycle_push(3);
            break;
        case ABSOLUTE: // 0xEC
            input_address = M6502_get_word(computer, program_counter, increment_true);
            result = x_register - memory_address[input_address];

            if(x_register >= memory_address[input_address]) set_flag(computer, CARRY);
            if(x_register == memory_address[input_address]) set_flag(computer, ZERO);
            check_flag_N(computer, result);
            cycle_push(4);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Compare Y register
void CPY(struct M6502* computer, uchar8_t mode){

    ushort16_t result;
    ushort16_t input_address;
    switch(mode)
    {
        case IMMEDIATE: // 0xC0
            result = y_register - memory_address[program_counter];
            if(y_register >= memory_address[program_counter]) set_flag(computer, CARRY);
            if(y_register == memory_address[program_counter]) set_flag(computer, ZERO);
            check_flag_N(computer, result);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xC4
            input_address = memory_address[program_counter];
            result = y_register - memory_address[input_address];

            if(y_register >= memory_address[input_address]) set_flag(computer, CARRY);
            if(y_register == memory_address[input_address]) set_flag(computer, ZERO);
            check_flag_N(computer, result);
            cycle_push(3);
            break;
        case ABSOLUTE: // 0xCC
            input_address = M6502_get_word(computer, program_counter, increment_true);
            result = y_register - memory_address[input_address];

            if(y_register >= memory_address[input_address]) set_flag(computer, CARRY);
            if(y_register == memory_address[input_address]) set_flag(computer, ZERO);
            check_flag_N(computer, result);
            cycle_push(4);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Decrement memory
void DEC(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    ushort16_t input_address_offset;
    ushort16_t memory_value;
    switch(mode)
    {
        case ZERO_PAGE: // 0xC6
            input_address = memory_address[program_counter];
            memory_value = memory_address[input_address] -= 1;
            M6502_set_memory(computer, input_address, memory_value);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0xD6
            input_address = memory_address[program_counter];
            input_address_offset = input_address + x_register;
            memory_value = memory_address[input_address_offset] -= 1;
            M6502_set_memory(computer, input_address_offset, memory_value);
            cycle_push(6);
            break;
        case ABSOLUTE: // 0xCE
            input_address = M6502_get_word(computer, program_counter, increment_true);
            memory_value = memory_address[input_address] -= 1;
            M6502_set_memory(computer, input_address, memory_value);
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0xDE
            input_address = M6502_get_word(computer, program_counter, increment_true);
            input_address_offset = input_address + x_register;
            memory_value = memory_address[input_address_offset] -= 1;
            M6502_set_memory(computer, input_address_offset, memory_value);
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Decrement X register
void DEX(struct M6502* computer){ // 0xCA
    x_register -= 1;
    check_flag_ZN(computer, x_register);
    cycle_push(2);
}

// Decrement Y register
void DEY(struct M6502* computer){ // 0x88
    y_register -= 1;
    check_flag_ZN(computer, y_register);
    cycle_push(2);
}

// Exclusive OR against byte of memory and accumulator
void EOR(struct M6502* computer, uchar8_t mode){

    uchar8_t input_value;
    ushort16_t input_address;
    ushort16_t input_address_offset;
    ushort16_t word_address;
    switch(mode)
    {
        case IMMEDIATE: // 0x49
            input_value = memory_address[program_counter];
            accumulator ^= input_value;
            check_flag_ZN(computer, accumulator); 
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x45
            input_address = memory_address[program_counter];
            // accumulator equals itself ORed against byte at memory
            accumulator ^= memory_address[input_address];
            check_flag_ZN(computer, accumulator); 
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x55
            input_address = memory_address[program_counter];
            input_address_offset = input_address + x_register;
            accumulator ^= memory_address[input_address_offset];
            check_flag_ZN(computer, accumulator); 
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x4D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            // accumulator ORed against byte at an absolute memory address
            accumulator ^= memory_address[input_address];
            check_flag_ZN(computer, accumulator); 
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x5D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            input_address_offset = input_address + x_register;
            accumulator ^= memory_address[input_address_offset];
            check_flag_ZN(computer, accumulator); 
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, x_register, 1);
            break;
        case ABSOLUTE_Y: // 0x59
            input_address = M6502_get_word(computer, program_counter, increment_true);
            input_address_offset = input_address + y_register;
            accumulator ^= memory_address[input_address_offset];
            check_flag_ZN(computer, accumulator); 
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, y_register, 1);
            break;
        case INDIRECT_X: // 0x41
            input_address = memory_address[program_counter];
            input_address_offset = input_address + x_register;
            word_address = M6502_get_word(computer, input_address_offset, increment_false);
            // accumulator equals itself ORed against the value at the word address
            accumulator ^= memory_address[word_address];
            check_flag_ZN(computer, accumulator); 
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x51
            input_address = memory_address[program_counter];
            word_address = M6502_get_word(computer, input_address, increment_false);
            input_address_offset = word_address + y_register;
            accumulator ^= memory_address[input_address_offset];
            check_flag_ZN(computer, accumulator); 
            cycle_push(5); // +1 if page crossed
            check_page(computer, word_address, y_register, 1);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Increment memory
void INC(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    ushort16_t input_address_offset;
    ushort16_t memory_value;
    switch(mode)
    {
        case ZERO_PAGE: // 0xE6
            input_address = memory_address[program_counter];
            memory_value = memory_address[input_address] += 1;
            M6502_set_memory(computer, input_address, memory_value);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0xF6
            input_address = memory_address[program_counter];
            input_address_offset = input_address + x_register;
            memory_value = memory_address[input_address_offset] += 1;
            cycle_push(6);
            break;
        case ABSOLUTE: // 0xEE
            input_address = M6502_get_word(computer, program_counter, increment_true);
            memory_value = memory_address[input_address] += 1;
            M6502_set_memory(computer, input_address, memory_value);
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0xFE
            input_address = M6502_get_word(computer, program_counter, increment_true);
            input_address_offset = input_address + x_register;
            memory_value = memory_address[input_address_offset] += 1;
            M6502_set_memory(computer, input_address_offset, memory_value);
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Increment X register
void INX(struct M6502* computer){ // 0xE8
    x_register += 1;
    check_flag_ZN(computer, x_register);
    cycle_push(2);
}

// Increment Y register
void INY(struct M6502* computer){ // 0xC8
    y_register += 1;
    check_flag_ZN(computer, y_register);
    cycle_push(2);
}

// Jump to address
void JMP(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    ushort16_t indirect_address;
    switch(mode)
    {
        case ABSOLUTE: // 0x4C
            // sets program counter to user address input
            input_address = M6502_get_word(computer, program_counter, increment_true);
            program_counter = input_address;
            cycle_push(3);
            break;
        case INDIRECT: // 0x6C
            // sets program counter to an indirect adress found at a users address input
            input_address = M6502_get_word(computer, program_counter, increment_true);
            indirect_address = M6502_get_word(computer, input_address, increment_false);
            program_counter = indirect_address;
            cycle_push(5);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Jump to Subroutine
void JSR(struct M6502* computer, uchar8_t mode){ // 0x20 - Absolute
    // First get subroutines address
    ushort16_t input_address = M6502_get_word(computer, program_counter, increment_true);
    // Extract high bit by shifting right 8 and low bit by truncating the 16 bits to char
    uchar8_t extract_msb = program_counter>>8;
    uchar8_t extract_lsb = (char)program_counter;
    M6502_stack_push(computer, extract_msb); // most significant byte
    M6502_stack_push(computer, extract_lsb); // least significant byte
    // set the program counter to supplied address, minus 1 keeps PC loop on track
    program_counter = input_address-1;
    cycle_push(6);
}

// load memory into accumulator
void LDA(struct M6502* computer, uchar8_t mode){

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
            offset_address = input_address + x_register;
            accumulator = memory_address[offset_address];
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
            offset_address = input_address + x_register;
            accumulator = memory_address[offset_address];
            //printf("Accumulator: %i\n", accumulator);
            check_flag_ZN(computer, accumulator);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, x_register, 1); // will cycle if page crossed
            break;
        }
        case ABSOLUTE_Y:{ // 0xB9
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + y_register;
            accumulator = memory_address[offset_address];
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

// load memory into X register
void LDX(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    ushort16_t offset_address;
    switch(mode)
    {
        case IMMEDIATE: // 0xA2
            x_register = memory_address[program_counter];
            check_flag_ZN(computer, x_register);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xA6
            input_address = memory_address[program_counter];
            x_register = memory_address[input_address];
            check_flag_ZN(computer, x_register);
            cycle_push(3);
            break;
        case ZERO_PAGE_Y: // 0xB6
            input_address = memory_address[program_counter];
            offset_address = input_address + y_register;
            y_register = memory_address[offset_address];
            check_flag_ZN(computer, x_register);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0xAE
            x_register = memory_address[M6502_get_word(computer, program_counter, increment_true)];
            check_flag_ZN(computer, x_register);
            cycle_push(4);
            break;
        case ABSOLUTE_Y: // 0xBE
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + y_register;
            x_register = memory_address[offset_address];
            check_flag_ZN(computer, x_register);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, y_register, 1); // will cycle if page crossed
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// load memory into Y register
void LDY(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    ushort16_t offset_address;
    switch(mode)
    {
        case IMMEDIATE: // 0xA0
            y_register = memory_address[program_counter];
            check_flag_ZN(computer, y_register);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xA4
            input_address = memory_address[program_counter];
            y_register = memory_address[input_address];
            check_flag_ZN(computer, y_register);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0xB4
            input_address = memory_address[program_counter];
            offset_address = input_address + x_register;
            y_register = memory_address[offset_address];
            check_flag_ZN(computer, y_register);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0xAC
            y_register = memory_address[M6502_get_word(computer, program_counter, increment_true)];
            check_flag_ZN(computer, y_register);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0xBC
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            y_register = memory_address[offset_address];
            check_flag_ZN(computer, y_register);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, x_register, 1); // will cycle if page crossed
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Logical shift right
void LSR(struct M6502* computer, uchar8_t mode){
    // should probably test these work
    // * need to work on carry flag 
    ushort16_t input_address;
    ushort16_t offset_address;
    ushort16_t m_result;
    
    switch(mode)
    {
        case ACCUMULATOR: // 0x4A
            // note using flag negative bit just the check 7th bit
            //accumulator ;
            // shift accumulator 1 bit to the right
            accumulator >>= 1;
            check_flag_ZN(computer, accumulator);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x46
            // shift the value provided at current location 1 bit to the right 
            m_result = memory_address[program_counter] >>= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x56
            offset_address = memory_address[program_counter] + x_register;
            // shift the value provided at this location 1 bit to the right 
            m_result = memory_address[offset_address] >>= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x4E
            // provides a 16 bit address to work from
            input_address = M6502_get_word(computer, program_counter, increment_true);
            // shift the value provided at this location 1 bit to the left 
            m_result = memory_address[input_address] >>= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x5E
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            m_result = memory_address[offset_address] >>= 1;
            check_flag_ZN(computer, m_result);
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// no operation
void NOP(struct M6502* computer){ // 0xEA
    cycle_push(2);
}

// logical inclusive OR performed on accumulator against memory
void ORA(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    ushort16_t offset_address;
    ushort16_t word_address;
    switch(mode)
    {
        case IMMEDIATE: // 0x09
            accumulator = accumulator | memory_address[program_counter];
            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, accumulator);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x05
            input_address = memory_address[program_counter];
            accumulator = accumulator | memory_address[input_address];
            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, accumulator);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x15
            input_address = memory_address[program_counter];
            offset_address = input_address + x_register;
            accumulator = accumulator | memory_address[offset_address];
            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, accumulator);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x0D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            accumulator = accumulator | memory_address[input_address];
            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, accumulator);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x1D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            accumulator = accumulator | memory_address[offset_address];
            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, accumulator);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, x_register, 1);
            break;
        case ABSOLUTE_Y: // 0x19
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + y_register;
            accumulator = accumulator | memory_address[offset_address];
            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, accumulator);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, y_register, 1);
            break;
        case INDIRECT_X: // 0x01
            // this one just takes a byte as an address
            input_address = M6502_get_byte(computer, program_counter);
            offset_address = input_address + x_register;
            word_address = M6502_get_word(computer, offset_address, increment_false);
            // OR operation against whats at the word address
            accumulator = accumulator | memory_address[offset_address];
            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, accumulator);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x11
            input_address = M6502_get_byte(computer, program_counter);
            word_address = M6502_get_word(computer, input_address, increment_true);
            offset_address = word_address + y_register;
            // OR operation against whats at the offset address
            accumulator = accumulator | offset_address;
            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, accumulator);
            cycle_push(5); // +1 if page crossed
            check_page(computer, input_address, y_register, 1);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Push accumulator to stack
void PHA(struct M6502* computer){ // 0x48
    M6502_stack_push(computer, accumulator);
    cycle_push(3);
}

// Push status register to stack
void PHP(struct M6502* computer){ // 0x08
    // note says something about break flag be and bit 5 set to 1
    M6502_stack_push(computer, status_register);
    cycle_push(3);
}

// Pop stack into the accumulator
void PLA(struct M6502* computer){ // 0x68
    accumulator = M6502_stack_pop(computer);
    check_flag_ZN(computer, accumulator);
    cycle_push(4);
}

// Pop stack into the status register, sets all flags with this value
void PLP(struct M6502* computer){ // 0x28
    // flags should be naturally set by num pulled here
    status_register = M6502_stack_pop(computer);
    cycle_push(4);
}

// Rotate bits right fill bit 0 with current carry flag and set carry flag with old bit 7
void ROL(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    ushort16_t offset_address;
    uchar8_t memory_value;
    uchar8_t flag_value;
    switch(mode)
    {
        case ACCUMULATOR: // 0x2A
            // returns current value of carry flag bit 
            flag_value = is_flag_set(CARRY, status_register);

            // returns old bit 7 of accumulator - sets carry flag if 1 else set carry to 0
            is_flag_set(NEGATIVE, accumulator) == 1 ? set_flag(computer, CARRY):clear_flag(computer, CARRY);

            accumulator <<= 1;

            // Uses the value from old carry flag to set bit 0 of accumulator
            flag_value == 1 ? set_bit(0, accumulator) : clear_bit(0, accumulator);

            // check flags for result
            check_flag_ZN(computer, accumulator);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x26
            flag_value = is_flag_set(CARRY, status_register);
            input_address = memory_address[program_counter];
            memory_value = memory_address[input_address];

            // returns bit 7 of provide value - sets carry flag if 1 else set carry to 0
            is_flag_set(NEGATIVE, memory_value) == 1 ? set_flag(computer, CARRY):clear_flag(computer, CARRY);

            memory_value <<=1;

            // Uses the value from old carry flag to set bit 0 of memory value
            flag_value == 1 ? set_bit(0, memory_value) : clear_bit(0, memory_value);

            check_flag_ZN(computer, memory_value);
            
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x36
            flag_value = is_flag_set(CARRY, status_register);
            input_address = memory_address[program_counter];
            offset_address = input_address + x_register;
            memory_value = memory_address[offset_address];

            // returns bit 7 of provide value - sets carry flag if 1 else set carry to 0
            is_flag_set(NEGATIVE, memory_value) == 1 ? set_flag(computer, CARRY):clear_flag(computer, CARRY);

            memory_value >>=1;

            // Uses the value from old carry flag to set bit 0 of memory value
            flag_value == 1 ? set_bit(0, memory_value) : clear_bit(0, memory_value);

            check_flag_ZN(computer, memory_value);
            
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x2E
            flag_value = is_flag_set(CARRY, status_register);
            input_address = M6502_get_word(computer, program_counter, increment_true);
            memory_value = memory_address[input_address];

            // returns bit 7 of provide value - sets carry flag if 1 else set carry to 0
            is_flag_set(NEGATIVE, memory_value) == 1 ? set_flag(computer, CARRY):clear_flag(computer, CARRY);

            memory_value >>=1;

            // Uses the value from old carry flag to set bit 0 of memory value
            flag_value == 1 ? set_bit(0, memory_value) : clear_bit(0, memory_value);

            check_flag_ZN(computer, memory_value);
            
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x3E
            flag_value = is_flag_set(CARRY, status_register);
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            memory_value = memory_address[offset_address];

            // returns bit 7 of provide value - sets carry flag if 1 else set carry to 0
            is_flag_set(NEGATIVE, memory_value) == 1 ? set_flag(computer, CARRY):clear_flag(computer, CARRY);

            memory_value >>=1;

            // Uses the value from old carry flag to set bit 0 of memory value
            flag_value == 1 ? set_bit(0, memory_value) : clear_bit(0, memory_value);

            check_flag_ZN(computer, memory_value);
            
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Rotate bits right fill bit 7 with current carry flag and set carry flag with old bit 0
void ROR(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    ushort16_t offset_address;
    uchar8_t memory_value;
    uchar8_t flag_value;
    switch(mode)
    {
        case ACCUMULATOR: // 0x6A
            // returns current value of carry flag bit 
            flag_value = is_flag_set(CARRY, status_register);

            // returns old bit 0 of accumulator - sets carry flag if 1 else set carry to 0
            is_flag_set(CARRY, accumulator) == 1 ? set_flag(computer, CARRY):clear_flag(computer, CARRY);

            accumulator >>= 1;

            // Uses the value from old carry flag to set bit 7 of accumulator
            flag_value == 1 ? set_bit(7, accumulator) : clear_bit(7, accumulator);

            // check flags for result
            check_flag_ZN(computer, accumulator);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0x66
            flag_value = is_flag_set(CARRY, status_register);
            input_address = memory_address[program_counter];
            memory_value = memory_address[input_address];

            // returns bit 0 of provide value - sets carry flag if 1 else set carry to 0
            is_flag_set(CARRY, memory_value) == 1 ? set_flag(computer, CARRY):clear_flag(computer, CARRY);

            memory_value >>=1;

            // Uses the value from old carry flag to set bit 7 of memory value
            flag_value == 1 ? set_bit(7, memory_value) : clear_bit(7, memory_value);

            check_flag_ZN(computer, memory_value);
            cycle_push(5);
            break;
        case ZERO_PAGE_X: // 0x76
            flag_value = is_flag_set(CARRY, status_register);
            input_address = memory_address[program_counter];
            offset_address = input_address + x_register;
            memory_value = memory_address[offset_address];

            // returns bit 0 of provide value - sets carry flag if 1 else set carry to 0
            is_flag_set(CARRY, memory_value) == 1 ? set_flag(computer, CARRY):clear_flag(computer, CARRY);

            memory_value >>=1;

            // Uses the value from old carry flag to set bit 7 of memory value
            flag_value == 1 ? set_bit(7, memory_value) : clear_bit(7, memory_value);

            check_flag_ZN(computer, memory_value);
            
            cycle_push(6);
            break;
        case ABSOLUTE: // 0x6E
            flag_value = is_flag_set(CARRY, status_register);
            input_address = M6502_get_word(computer, program_counter, increment_true);
            memory_value = memory_address[input_address];

            // returns bit 0 of provide value - sets carry flag if 1 else set carry to 0
            is_flag_set(CARRY, memory_value) == 1 ? set_flag(computer, CARRY):clear_flag(computer, CARRY);

            memory_value >>=1;

            // Uses the value from old carry flag to set bit 7 of memory value
            flag_value == 1 ? set_bit(7, memory_value) : clear_bit(7, memory_value);

            check_flag_ZN(computer, memory_value);
            
            cycle_push(6);
            break;
        case ABSOLUTE_X: // 0x7E
            flag_value = is_flag_set(CARRY, status_register);
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            memory_value = memory_address[offset_address];

            // returns bit 0 of provide value - sets carry flag if 1 else set carry to 0
            is_flag_set(CARRY, memory_value) == 1 ? set_flag(computer, CARRY):clear_flag(computer, CARRY);

            memory_value >>=1;

            // Uses the value from old carry flag to set bit 7 of memory value
            flag_value == 1 ? set_bit(7, memory_value) : clear_bit(7, memory_value);

            check_flag_ZN(computer, memory_value);
            
            cycle_push(7);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Return from Interrupt
void RTI(struct M6502* computer){ // 0x40
    status_register = M6502_stack_pop(computer); // this should pull Status Register
    uchar8_t lsb = M6502_stack_pop(computer); // pop least significant byte for PC
    uchar8_t msb = M6502_stack_pop(computer); // pop most significant byte for PC
    ushort16_t return_address = msb << 8 | lsb; // combine to make 16 bit address
    program_counter = return_address; // set program counter to return address
    printf("Return to address: %04X\n", program_counter);
    cycle_push(6);
}

// Return from Subroutine
void RTS(struct M6502* computer){ // 0x60
    uchar8_t lsb = M6502_stack_pop(computer); // pop least significant byte
    uchar8_t msb = M6502_stack_pop(computer); // pop most significant byte
    ushort16_t return_address = msb << 8 | lsb; // combine to make 16 bit address
    program_counter = return_address; // set program counter to return address
    printf("Return to address: %04X\n", program_counter);
    cycle_push(6);
}

// Subtract with Carry
void SBC(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    ushort16_t offset_address;
    ushort16_t word_address;
    short16_t short_result;
    uchar8_t flag_bit;
    switch(mode)
    {
        case IMMEDIATE: // 0xE9
            // returns if flag bit is set
            flag_bit = is_flag_set(CARRY, status_register);
            // check if this is same response as adc
            short_result = accumulator - memory_address[program_counter] - flag_bit;
            accumulator = accumulator - memory_address[program_counter] - flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(2);
            break;
        case ZERO_PAGE: // 0xE5
            input_address = memory_address[program_counter];
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator - memory_address[input_address] - flag_bit;
            accumulator = accumulator - memory_address[input_address] - flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0xF5
            input_address = memory_address[program_counter];
            offset_address = input_address + x_register;
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator - memory_address[offset_address] - flag_bit;
            accumulator = accumulator - memory_address[offset_address] - flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(4);
            break;
        case ABSOLUTE: // 0xED
            input_address = M6502_get_word(computer, program_counter, increment_true);
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator - memory_address[input_address] - flag_bit;
            accumulator = accumulator - memory_address[input_address] - flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0xFD
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator - memory_address[offset_address] - flag_bit;
            accumulator = accumulator - memory_address[offset_address] - flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, x_register, 1);
            break;
        case ABSOLUTE_Y: // 0xF9
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + y_register;
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator - memory_address[offset_address] - flag_bit;
            accumulator = accumulator - memory_address[offset_address] - flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(4); // +1 if page crossed
            check_page(computer, input_address, y_register, 1);
            break;
        case INDIRECT_X: // 0xE1
            input_address = memory_address[program_counter];
            offset_address = input_address + x_register;
            word_address = M6502_get_word(computer, offset_address, increment_true);
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator - memory_address[word_address] - flag_bit;
            accumulator = accumulator - memory_address[word_address] - flag_bit;

            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0xF1
            input_address = M6502_get_byte(computer, program_counter);
            word_address = M6502_get_word(computer, input_address, increment_true);
            offset_address = word_address + y_register;   
            flag_bit = is_flag_set(CARRY, status_register);
            short_result = accumulator - memory_address[offset_address] - flag_bit;
            accumulator = accumulator - memory_address[offset_address] - flag_bit;
            
            check_flag_ZN(computer, accumulator);
            check_flag_Carry(computer, short_result);
            cycle_push(5); // +1 if page crossed
            check_page(computer, input_address, y_register, 1);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// set carry flag
void SEC(struct M6502* computer){ // 0x38
    set_flag(computer, CARRY);
    cycle_push(2);
}

// set decimal flag
void SED(struct M6502* computer){ // 0xF8
    set_flag(computer, DECIMAL);
    cycle_push(2);
}

// set interrupt flag
void SEI(struct M6502* computer){ // 0x78
    set_flag(computer, INTERRUPT);
    cycle_push(2);
}

// Store Accumulator into memory
void STA(struct M6502* computer, uchar8_t mode){

    ushort16_t input_address;
    ushort16_t offset_address;
    ushort16_t word_address;
    switch(mode)
    {
        case ZERO_PAGE: // 0x85
            memory_address[program_counter] = accumulator;
            cycle_push(3);
            break;
        case ZERO_PAGE_X: // 0x95
            input_address = memory_address[program_counter] + x_register;
            memory_address[input_address] = accumulator;
            cycle_push(4);
            break;
        case ABSOLUTE: // 0x8D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            memory_address[input_address] = accumulator;
            cycle_push(4);
            break;
        case ABSOLUTE_X: // 0x9D
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + x_register;
            memory_address[offset_address] = accumulator;
            cycle_push(5);
            break;
        case ABSOLUTE_Y: // 0x99
            input_address = M6502_get_word(computer, program_counter, increment_true);
            offset_address = input_address + y_register;
            memory_address[offset_address] = accumulator;
            cycle_push(5);
            break;
        case INDIRECT_X: // 0x81
            input_address = M6502_get_byte(computer, program_counter);
            offset_address = input_address + x_register;
            word_address = M6502_get_word(computer, offset_address, increment_true);
            memory_address[word_address] = accumulator;
            cycle_push(6);
            break;
        case INDIRECT_Y: // 0x91
            input_address = M6502_get_byte(computer, program_counter);
            word_address = M6502_get_word(computer, input_address, increment_true);
            offset_address = word_address + y_register;
            memory_address[offset_address] = accumulator;
            cycle_push(6);
            break;
        default:
            puts("Error: Please specify addressing mode");
            break;
    }
}

// Store X register into memory
void STX(struct M6502* computer, uchar8_t mode){

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

// Store Y register into memory
void STY(struct M6502* computer, uchar8_t mode){

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

// transfer accumulator to x register
void TAX(struct M6502* computer){ // 0xAA
    x_register = accumulator;
    check_flag_ZN(computer, x_register);
    cycle_push(2);
}

// transfer accumulator to y register
void TAY(struct M6502* computer){ // 0xA8
    y_register = accumulator;
    check_flag_ZN(computer, y_register);
    cycle_push(2);
}

// transfer stack pointer to x register
void TSX(struct M6502* computer){ // 0xBA
    x_register = stack_pointer;
    check_flag_ZN(computer, x_register);
    cycle_push(2);
}

// transfer x register to accumulator
void TXA(struct M6502* computer){ // 0x8A
    accumulator = x_register;
    check_flag_ZN(computer, accumulator);
    cycle_push(2);
}

// transfer x register to stack pointer
void TXS(struct M6502* computer){ // 0x9A
    stack_pointer = x_register;
    cycle_push(2);
}

// transfer y register to accumulator
void TYA(struct M6502* computer){ // 0x98
    accumulator = y_register;
    check_flag_ZN(computer, accumulator);
    cycle_push(2);
}
