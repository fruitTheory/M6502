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
