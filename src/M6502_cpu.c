#include "M6502_cpu.h"
#include <stdio.h>

ushort16_t instruction_byte_count;
ulong64_t cycles;

extern inline void cycle_push(uchar8_t cycle);

ulong64_t cycle_current(){
    printf("current cycle: %li\n", cycles);
    return cycles;
}