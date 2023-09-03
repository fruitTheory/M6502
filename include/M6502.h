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
void check_page(struct M6502* computer, ushort16_t input_address, uchar8_t register_n);

// A container used to store the amount of bytes internal instructions use
// It is used to subtract from the total program size
// Subtracting from this adds bytes to amount of outputted instructions
extern ushort16_t instruction_byte_count;
// Total number of CPU cycles
extern ulong64_t cycles;

inline void cycle_push(uchar8_t cycle){
    cycles += cycle;
}
ulong64_t cycle_current();

// increment program counter by + 1
inline void PC_increment(struct M6502* computer){
    program_counter += 1;
    instruction_byte_count += 1;
}

// decrement program counter by - 1
inline void PC_decrement(struct M6502* computer){
    program_counter -= 1;
    instruction_byte_count -= 1;
}

