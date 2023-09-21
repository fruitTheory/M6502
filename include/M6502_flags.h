#pragma once

#include "config.h"

struct M6502; // forward declaration

void set_flag(struct M6502* computer, uchar8_t FLAG);
void clear_flag(struct M6502* computer, uchar8_t FLAG);
void check_flag_Carry(struct M6502* computer, short16_t test_against);
void check_flag_Z(struct M6502* computer, uchar8_t test_against);
void check_flag_N(struct M6502* computer, uchar8_t test_against);
void check_flag_ZN(struct M6502* computer, uchar8_t test_against);
uchar8_t is_flag_set(uchar8_t FLAG, uchar8_t test_against);
uchar8_t set_bit(uchar8_t bit, uchar8_t* test_against);
uchar8_t clear_bit(uchar8_t bit, uchar8_t* test_against);
uchar8_t check_bit(uchar8_t bit, uchar8_t test_against);