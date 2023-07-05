#pragma once

#include "MOS_6502_registers.h"
#include "MOS_6502_stack.h"
#include "MOS_6502_memory.h"

struct MOS_6502{

    struct MOS_6502_registers registers;
    struct MOS_6502_stack stack;
    struct MOS_6502_memory memory;
    
};