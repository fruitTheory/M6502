#pragma once
#include <stdbool.h>


/*
-------hardware interrupts -------
*/

bool external_reset_button(bool pushed); // system reset if user chooses to
bool critical_tempature(); // if system is overheating 


/*
------- reserved memory mapped handlers - hardware interrupts -------
*/

void non_maskable_interrupt();
void reset_vector();


/*
------- software interrupt -------
*/

void interrupt_request();