#include "hardware_interrupts.h"


bool external_reset_button(bool pushed){
    if(pushed)
        system_interrupt();
    // reset to a known state

}


/*
0xFFFC - 0xFFFD
These two bytes are reserved for critical system interrupts

What triggers an NMI?

External Hardware Events: For example, the vertical refresh of video display. Which gives system a chance to update graphics

Critical Errors: Hardware malfunctions or other critical events might trigger an NMI to ensure the system responds immediately

- This is a very low level process to where raw hardware takes over, transisters, gates, etc.
*/
void non_maskable_interrupt()
{
    // These hi-low address contain the address where CPU should start executing the NMI handler routine
    0xFFFA; // 65530
    0xFFFB; // 65531
    // If address is 0952 due to little-endian it would be stored FFFA = 52, FFFB = 09
    
    // if (system_intterupt())
    //     do_NMI_handler_routine(); //routine generally ends with RTI (Return from Interrupt) instruction
    //     return_from_interrupt();{
    //         restore_PC();
    //         restore_status_flags();};
    // We need to emulate this! were emulating a machine, but not using the literal machine at all - its abstracted
            
};

// 0xFFFC - 0xFFFD
// These two bytes are the reset vector and point to an initial address on startup
// This is normally determined by a bootloader, for ours we'll use $0000 as the origin
void reset_vector()
{
    0xFFFC; // 65532
    0xFFFD; // 65533
};

// 0xFFFE - 0xFFFF
// These two bytes are referred to as the break/interrupt request handler
void interrupt_request(){
    0xFFFE; // 65534
    0xFFFF; // 65535
}

/*
BRK initiates a software interrupt similar to a hardware
interrupt (IRQ). The return address pushed to the stack is
PC+2, providing an extra byte of spacing for a break mark
(identifying a reason for the break.)
The status register will be pushed to the stack with the break
flag set to 1. However, when retrieved during RTI or by a PLP
instruction, the break flag will be ignored.
The interrupt disable flag is not set automatically.
*/

