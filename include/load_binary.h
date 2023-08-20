#include "MOS_6502.h"

uchar8_t* load_program(int argc, char* argv[]);
void print_file_info(int argc, char* argv[]);

uchar8_t MOS_6502_memory_get(MOS_6502_memory* memory, int location);
ushort16_t MOS_6502_memory_get_short(MOS_6502_memory* memory, int location);