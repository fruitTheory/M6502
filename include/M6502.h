#pragma once

#include "M6502_registers.h"
#include "M6502_stack.h"
#include "M6502_memory.h"
#include <stddef.h>

// M6502 type - registers, stack, memory
struct M6502{

    M6502_registers registers;
    M6502_stack stack;
    M6502_memory memory;
    
};

void M6502_init(struct M6502* M6502);
void store_program(struct M6502* M6502, uchar8_t* buffer, size_t program_size);
void execute_instruction(struct M6502* M6502, uchar8_t opcode);