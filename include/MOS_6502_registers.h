#pragma once

#include "config.h"

typedef struct 
{
    unsigned short PC;  // program counter - 16 bit - points to the next instruction to be executed
    unsigned char AC;   // accumulator - 8 bit - used for all arithmetic and logical operations (with the exception of increments and decrements)
    unsigned char X;    // X register - 8 bit - most commonly used to hold counters or offsets for accessing memory
    unsigned char Y;    // Y register - 8 bit - available for holding counter or offsets memory access and supports the same set of memory load, save and compare operations as wells as increments and decrements
    unsigned char SR;   // status register - 8 bit - stores status flags
    unsigned char SP;   // stack pointer - 8 bit - holds the low 8 bits of the next free location on the stack

}MOS_6502_registers;

// 8 bits of processor status flags stored in status register
enum SR_Flags{
    Carry, // Set if the last operation carried (addition) or borrowed (subtraction)
    Zero, //  Set if the result of the operation is zero, otherwise clear
    Interrupt, // If set, disables all maskable interrupts
    Decimal, // Used to enable Binary Coded Decimal (BCD) mode in the processor, ignored in 8-bit 6502s
    Break, // Only appears when a BRK instruction has been executed and an interrupt has been generated
    Ignored, // Ignored
    Overflow, //  Set if the signed result overflows, otherwise clear
    Negative // Set if the result is negative, clear if positive
};