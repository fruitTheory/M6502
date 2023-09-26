#include "hardware_interrupts.h"
#include "M6502_flags.h"
#include <stdio.h>

/*
0xFFFC - 0xFFFD
These two bytes are reserved for critical system interrupts

What triggers an NMI?

External Hardware Events: For example, the vertical refresh of video display. Which gives system a chance to update graphics

Critical Errors: Hardware malfunctions or other critical events might trigger an NMI to ensure the system responds immediately

- This is a very low level process to where raw hardware takes over, transisters, gates, etc.
*/
void non_maskable_interrupt(struct M6502* computer){
    // 0xFFFA; // 65530 - contains low byte
    // 0xFFFB; // 65531 - contains high byte
    ushort16_t nmi_address = cpu_get_word(computer, 0xFFFA, increment_false);
    program_counter -= 1; // important
    cpu_stack_push(computer, program_counter>>8); // push high byte
    cpu_stack_push(computer, program_counter); // push low byte
    clear_flag(computer, BREAK);
    cpu_stack_push(computer, status_register);

    program_counter = nmi_address;
    //RTI(computer);

};

// 0xFFFC - 0xFFFD
// These two bytes are the reset vector and point to an initial address on startup
// This is normally determined by a bootloader, for emu typically is $8000
void reset_vector(struct M6502* computer){
    // 0xFFFC; // 65532
    // 0xFFFD; // 65533
    ushort16_t reset_address = cpu_get_word(computer, 0xFFFC, increment_false);
    printf("Resetting Computer to: %04X\n", reset_address);
    M6502_init(computer);
    program_counter = reset_address;
};

/*
0xFFFE - 0xFFFF
These bytes are referred to as the break/interrupt request handler

This request is maskable and can be masked by setting interrupt disable flag 
set with SEI (Set Interrupt Disable) instruction

Unlike NMI, IRQs can be triggered at various locations, such as by APU and Mappers
*/
void interrupt_request(struct M6502* computer){
    // 0xFFFE; // 65534
    // 0xFFFF; // 65535
    ushort16_t interrupt_address = cpu_get_word(computer, 0xFFFE, increment_false);
    program_counter -= 1;
    cpu_stack_push(computer, program_counter>>8); // push high byte
    cpu_stack_push(computer, program_counter); // push low byte
    clear_flag(computer, BREAK);
    cpu_stack_push(computer, status_register);
    program_counter = interrupt_address;

}

// hardware reset to a known state
void external_reset_button(struct M6502* computer){
    reset_vector(computer);
}


/*
As an interrupt occurs, any instruction currently processed is completed first.
The registers stored on the stack are recovered at the end of an
interrupt routine, as control is transferred back to the interrupted code by
the RTI instruction.
*/

/*
BRK 
The status register will be pushed to the stack with the break
flag set to 1. However, when retrieved during RTI or by a PLP
the break flag will be ignored.
The interrupt disable flag is not set automatically.
*/