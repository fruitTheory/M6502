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


// A container for bytes to help know when program instruction ends -
// It is the total amount of bytes needed by executed instructions
extern ushort16_t instruction_byte_count;
// Total number of CPU cycles
extern ulong64_t cycles;

inline void cycle_push(uchar8_t cycle){
    cycles += cycle;
}
ulong64_t cycle_current();


