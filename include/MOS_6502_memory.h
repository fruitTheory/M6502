#pragma once

#include "config.h"

typedef struct
{
    unsigned short memory[max_memory];
}MOS_6502_memory;

void set_memory(int index);
unsigned short get_memory(int index);
