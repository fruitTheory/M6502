#include "config.h"

struct M6502; // forward declaration

void set_flag(struct M6502* computer, uchar8_t FLAG);
void set_flags_all(struct M6502* computer);
void clear_flag(struct M6502* computer, uchar8_t FLAG);
void check_flag_Carry(struct M6502* computer, short16_t test_against);
void check_flag_Z(struct M6502* computer, uchar8_t test_against);
void check_flag_N(struct M6502* computer, uchar8_t test_against);
void check_flag_ZN(struct M6502* computer, uchar8_t test_against);
int is_flag_set(struct M6502* computer, uchar8_t FLAG);