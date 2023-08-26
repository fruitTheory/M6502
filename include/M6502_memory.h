#pragma once

#include "config.h"
#include <stddef.h>


typedef struct
{
    // including RAM, ROM, and memory-mapped I/O.
    uchar8_t address[max_memory];
    // please look into this making it ushort instead uchar doubles size of computer
}M6502_memory;

struct M6502; // forward declaration so compiler knows the struct exists and will be defined later

void M6502_set_memory(struct M6502* computer, ushort16_t location, uchar8_t value);
void M6502_memory_inbounds(ushort16_t index);

uchar8_t M6502_memory_get_byte(struct M6502* computer, ushort16_t location);
ushort16_t M6502_memory_get_word(struct M6502* computer, ushort16_t location);

void M6502_store_program(struct M6502* computer, uchar8_t* file, size_t program_size);

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