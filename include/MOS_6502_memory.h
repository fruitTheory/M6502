#pragma once

#include "config.h"

typedef struct
{
    // including RAM, ROM, and memory-mapped I/O.
    ushort16_t address[max_memory];
}MOS_6502_memory;

void set_memory(int index);
void MOS_6502_memory_inbounds(int index);

// uchar8_t MOS_6502_memory_get(MOS_6502_memory* memory, int location);
// ushort16_t MOS_6502_memory_get_short(MOS_6502_memory* memory, int location);

// memory map (reservered)

/*
The first 256 byte page of memory ($0000-$00FF) is referred to as 'Zero Page' and is the focus of a number of special 
addressing modes that result in shorter (and quicker) instructions or allow indirect access to the memory.
*/

// The second page of memory ($0100-$01FF) is reserved for the system stack and which cannot be relocated.

/*
The only other reserved locations in the memory map are the very last 6 bytes of memory $FFFA to $FFFF which must be
programmed with the addresses of the non-maskable interrupt handler ($FFFA/B), the power on reset location ($FFFC/D)
and the BRK/interrupt request handler ($FFFE/F) respectively.
*/

// 0x00 BRK - Break/Interrupt - Address implied