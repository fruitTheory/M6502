#ifndef MOS_6502_H
#define MOS_6502_H

#include "MOS_6502_registers.h"

struct MOS_6502{

    struct MOS_6502_registers registers;
    struct MOS_6502_stack stack;
    
};

#endif