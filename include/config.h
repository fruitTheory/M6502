#pragma once

#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1

#define max_memory 65535 // 6502's 16-bit address bus can only handle this much
#define stack_max_size 256

#define zero_page_start 0x0000
#define stack_start 0x0100 // decimal(byte) 256 - Start of stack
#define stack_end 0x01FF // decimal(byte) 511 - Use as offset for programs
#define program_initial_load 0x0200 // decimal(byte) 512 - Programs first byte

// will need 16 at some point to represent data transfer amount and such