#include <stdio.h>
#include "M6502.h"
#include "config.h"
#include "M6502_registers.h"


// Progams should start at 0x0200
$ORG = program_initial_load;


void set_flag();
void CMP(struct M6502* M6502, uchar8_t user_value);

int main(int argc, char* argv[]){


    struct M6502 M6502;

    M6502.registers.AC = 0xFF;

    M6502_init(&M6502);
    
    uchar8_t user_value = 0xF;
    M6502.registers.PC = 0x00FA;
    printf("%04x\n", M6502.memory.address[M6502.registers.PC]);

    CMP(&M6502, user_value);
    execute_instruction(&M6502, 0x00);

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
void LDA(struct M6502* M6502, uchar8_t user_byte){
    
}


void CMP(struct M6502* M6502, uchar8_t user_value){

    if(M6502->registers.AC >= user_value){
        //set_carry_flag();
        printf("Carry flag is set\n");
    }
    if(M6502->registers.AC == user_value){
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
void set_flag(struct M6502 accumulator, uchar8_t user_value){

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