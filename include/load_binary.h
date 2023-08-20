#include "MOS_6502.h"

char* load_binary(int argc, char* argv[]);


uchar8_t MOS_6502_memory_get(MOS_6502_memory* memory, int location);
ushort16_t MOS_6502_memory_get_short(MOS_6502_memory* memory, int location);