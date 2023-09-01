#pragma once

#include "config.h"

typedef struct 
{
    ushort16_t PC;  // program counter - 16 bit - points to the next instruction to be executed
    uchar8_t AC;   // accumulator - 8 bit - used for all arithmetic and logical operations (with the exception of increments and decrements)
    uchar8_t X;    // X register - 8 bit - most commonly used to hold counters or offsets for accessing memory
    uchar8_t Y;    // Y register - 8 bit - available for holding counter or offsets memory access and supports the same set of memory load, save and compare operations as wells as increments and decrements
    uchar8_t SR;   // status register - 8 bit - stores status flags
    ushort16_t SP;   // stack pointer - 8 bit - holds the low 8 bits of the next free location on the stack - (changed to 16 bit but clamped at 8 bit)

}M6502_registers;
