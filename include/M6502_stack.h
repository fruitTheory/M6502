#pragma once

#include "config.h"


struct M6502;

// 256 byte stack located between $0100 and $01FF
typedef struct 
{
    uchar8_t stack[stack_max_size];
}CPU_stack;

void cpu_stack_init(struct M6502* computer);
void cpu_stack_push(struct M6502* computer, uchar8_t push_value);
uchar8_t cpu_stack_pop(struct M6502* computer);