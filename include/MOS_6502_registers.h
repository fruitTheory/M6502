#pragma once

#include "config.h"

struct MOS_6502_registers
{
    unsigned short PC;  // program counter
    unsigned char AC;   // accumulator
    unsigned char X;    // X register
    unsigned char Y;    // Y register
    unsigned char SR;   // status register - P register
    unsigned char SP;   // stack pointer

};

enum SR_Flags{
    Carry,
    Zero,
    Interrupt,
    Decimal,
    Break,
    Ignored,
    Overflow,
    Negative
};