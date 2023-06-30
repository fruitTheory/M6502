#pragma once

#include "config.h"

struct MOS_6502_memory
{
    unsigned short memory[max_memory];
};

void set_memory();
unsigned short get_memory();
