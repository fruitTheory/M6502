#pragma once

#include "config.h"

// forward declaration of MOS_6502 struct
struct MOS_6502;

// 256 byte stack located between $0100 and $01FF
typedef struct 
{
    unsigned char stack[stack_max_size];
}MOS_6502_stack;

void MOS_6502_stack_init(struct MOS_6502* MOS_6502);

void MOS_6502_stack_push(struct MOS_6502* MOS_6502, unsigned short val);
void MOS_6502_stack_pop(struct MOS_6502* MOS_6502, unsigned short val);