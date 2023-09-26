#pragma once

#include "config.h"
#include "M6502.h"

uchar8_t get_pattern();
void test_prog(struct M6502* computer);
void PPU_register_handler(struct M6502* computer, ushort16_t address, uchar8_t value, char8_t rw);