#include <stdio.h>
#include "config.h"
#include "MOS_6502_registers.h"
#include "MOS_6502.h"

// Progams should start at 0x0200
$ORG = program_initial_load;

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
    
    if (system_intterupt())
        do_NMI_handler_routine(); //routine generally ends with RTI (Return from Interrupt) instruction
        return_from_interrupt();{
            restore_PC();
            restore_status_flags();};
            
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

void set_flag();
void CMP(struct MOS_6502* MOS_6502, uchar8_t user_value);

int main(int argc, char* argv[]){


    struct MOS_6502 MOS_6502;

    MOS_6502.registers.AC = 0xFF;
    uchar8_t user_value = 0xF;


    CMP(&MOS_6502, user_value);


    if(0xC0 & flag_negative_bit){
        printf("this is true");
    } else{
        printf("this is not true");
    }

    return EXIT_SUCCESS;

}


//Note: not quite as addressing modes yet, first get - opcodes - program counter - memory architecture 
/*
For load byte (LDA) in fact for any that need negative flag set if bit 7 of A is set
the highest one can get without bit 7 set is 127d so 128 bytes - 0111 1111 --
so if its greater than decimal 127 we can assume bit 7 of A is set, correct?

Note that its signed so -128-127 - self note: 0 counts as a poistive number

void LDA()
*/

/*
* Note on flags, examples set zero flag if == 0, set negative flag if negative << 7
*/
void LDA(struct MOS_6502* MOS_6502, uchar8_t user_byte){
    
}


void CMP(struct MOS_6502* MOS_6502, uchar8_t user_value){

    if(MOS_6502->registers.AC >= user_value){
        //set_carry_flag();
        printf("Carry flag is set\n");
    }
    if(MOS_6502->registers.AC == user_value){
        //set_zero_flag();
        printf("Zero flag is set\n");
    }

}

/*
    ORG $8000       ; Start address

START:
    LDA #$05       ; Load accumulator with the value 5
    CMP #$03       ; Compare accumulator with the value 3
    BCS GREATER    ; If A >= 3, branch to GREATER label
    STY $0200      ; If not, store Y register value (whatever it might be) to memory address $0200
    BRK            ; End of program

GREATER:
    STX $0201      ; Store X register value (whatever it might be) to memory address $0201
    BRK            ; End of program

*/

/*
void set_flag(struct MOS_6502 accumulator, uchar8_t user_value){

    // C	Carry Flag	Set if A >= M
    // Z	Zero Flag	Set if A = M

   printf("Something\n");

};
*/

/*
CMP - 193-221 - 19 bytes - opcode - C9 for immeadiate
Compare Memory with Accumulator

A - M
N	Z	C	I	D	V
+	+	+	-	-	-

if(result & flag_negative_bit)

*/