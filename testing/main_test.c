#include <stdio.h>
#include "M6502.h"
#include "config.h"
#include "M6502_registers.h"


// Progams should start at 0x0200
$ORG = program_initial_load;

int main(int argc, char* argv[]){


    struct M6502 computer;
    
    return EXIT_SUCCESS;

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