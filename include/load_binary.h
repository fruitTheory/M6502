#include "M6502.h"

ushort16_t get_program_size(char* argv[]);
uchar8_t* load_program(int argc, char* argv[]);
void print_program_info(int argc, char* argv[]);


// uchar8_t M6502_memory_get(ushort16_t* memory, ushort16_t location);
// ushort16_t M6502_memory_get_short(ushort16_t* memory, ushort16_t location);