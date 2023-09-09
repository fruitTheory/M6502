#pragma once

#include "M6502_cpu.h"
#include "ppu.h"

// M6502 based Computer - NES Variant
// CPU: registers, stack, memory
// PPU: registers, memory
struct M6502{
    M6502_cpu cpu;
    NES_ppu ppu;
};

void M6502_init(struct M6502* computer);
uchar8_t instruction_fetch(struct M6502* computer);
void analyze_opcode(struct M6502* computer, uchar8_t opcode);
void execute_instruction(struct M6502* computer, ushort16_t program_size);
void check_page(struct M6502* computer, ushort16_t input_address, uchar8_t register_n, uchar8_t cycles);


// increment program counter by + 1
inline void PC_increment(struct M6502* computer){
    program_counter += 1;
}

// decrement program counter by - 1
inline void PC_decrement(struct M6502* computer){
    program_counter -= 1;
}