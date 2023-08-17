#pragma once

#include "MOS_6502_registers.h"
#include "MOS_6502_stack.h"
#include "MOS_6502_memory.h"
#include <stddef.h>

// MOS_6502 type - registers, stack, memory
struct MOS_6502{

    MOS_6502_registers registers;
    MOS_6502_stack stack;
    MOS_6502_memory memory;
    
};

void MOS_6502_init(struct MOS_6502* MOS_6502);
void M0S_6502_program_load(struct MOS_6502* MOS_6502, const char* buffer, size_t program_size);
void execute_instruction(struct MOS_6502* MOS_6502, int opcode);