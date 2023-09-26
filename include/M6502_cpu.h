#pragma once

#include "M6502_registers.h"
#include "M6502_stack.h"
#include "M6502_memory.h"

typedef struct{
    CPU_registers registers;
    CPU_stack stack;
    CPU_memory memory;
}M6502_cpu;

// A container used to store the amount of bytes internal instructions use
// It is used to subtract from the total program size
// Subtracting from this adds bytes to amount of outputted instructions
extern ushort16_t instruction_byte_count; // May be obsolete now
// Total number of CPU cycles
extern ulong64_t cycles;

inline void cycle_push(uchar8_t cycle){
    cycles += cycle;
}
ulong64_t cycle_current();
