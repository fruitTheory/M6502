#pragma once

#include "config.h"


struct M6502;

// 256 byte stack located between $0100 and $01FF
typedef struct 
{
    uchar8_t stack[stack_max_size];
}M6502_stack;

void M6502_stack_init(struct M6502* M6502);
void M6502_stack_push(struct M6502* M6502, ushort16_t val);
void M6502_stack_pop(struct M6502* M6502, ushort16_t val);