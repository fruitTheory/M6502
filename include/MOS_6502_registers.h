#pragma once

#include "config.h"

struct MOS_6502_registers
{
    unsigned short PC;  // program counter - 16 bit
    unsigned char AC;   // accumulator - 8 bit
    unsigned char X;    // X register - 8 bit
    unsigned char Y;    // Y register - 8 bit
    unsigned char SR;   // status register - 8 bit
    unsigned char SP;   // stack pointer - 8 bit

};

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