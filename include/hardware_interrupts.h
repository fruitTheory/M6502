#pragma once
#include "config.h"
#include "M6502.h"
/*
------- reserved memory mapped handlers - hardware interrupts -------
*/

void non_maskable_interrupt(struct M6502* computer);
void reset_vector(struct M6502* computer);
void external_reset_button(struct M6502* computer); // system reset if user chooses to

/*
------- software interrupts -------
*/

void interrupt_request(struct M6502* computer);