#include "config.h"

struct M6502; // forward declaration

void set_flag(struct M6502* computer, uchar8_t FLAG);
void set_flags_all(struct M6502* computer);
void check_flag_ZN(struct M6502* computer, uchar8_t test_against);
void check_flag(struct M6502* computer, uchar8_t FLAG, uchar8_t test_against);