#include "M6502_cpu.h"
#include <stdio.h>

ulong64_t cycles;

extern inline void cycle_push(uchar8_t cycle);

ulong64_t cycle_current(){
    printf("current cycle: %li\n", cycles);
    return cycles;
}