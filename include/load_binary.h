#pragma once

ushort16_t get_program_size(char* argv[]);
uchar8_t* load_program(int argc, char* argv[]);
void print_program_info(int argc, char* argv[]);
uchar8_t* load_program_temporary(const char8_t* path);
void print_program_info_temporary(const char8_t* path);
ushort16_t get_program_size_temporary(const char8_t* path);
