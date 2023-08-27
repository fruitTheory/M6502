#pragma once

#include "M6502_registers.h"
#include "M6502_stack.h"
#include "M6502_memory.h"


// M6502 type - registers, stack, memory
struct M6502{

    M6502_registers registers;
    M6502_stack stack;
    M6502_memory memory;
    
};

void M6502_init(struct M6502* computer);
uchar8_t instruction_fetch(struct M6502* computer);
void analyze_opcode(struct M6502* computer, uchar8_t opcode);


extern uchar8_t flag_bit;

#define flag_negative_bit 0x80 // check bit 7 - 1000 0000
#define flag_overflow_bit 0x40 // check bit 6 - 0100 0000
#define flag_ignored_bit 0x20 // check bit 5 - 0010 0000
#define flag_break_bit 0x10 // check bit 4 - 0001 0000
#define flag_decimal_bit 0x08 // check bit 3 - 0000 1000
#define flag_interrupt_bit 0x04 // check bit 2 - 0000 0100
#define flag_zero_bit 0x02 // check bit 1 - 0000 0010
#define flag_carry_bit 0x01 // check bit 0 - 0000 0001
// if(flag_bit & flag_carry_bit)