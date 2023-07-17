#include <stdio.h>
#include "config.h"
#include "MOS_6502_registers.h"
#include "MOS_6502.h"

void set_flag();
void CMP(struct MOS_6502* MOS_6502, uchar8_t user_value);

int main(int argc, char* argv[]){

    printf("hey this works\n");


    printf("flag number: %i\n", flag_negative_bit);


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

void CMP(struct MOS_6502* MOS_6502, uchar8_t user_value){

    if(MOS_6502->registers.AC == user_value){
        printf("This is equivalent to Accumulator\n");
    }

    if(MOS_6502->registers.AC >= user_value){
        //set_carry_flag();
        printf("Carry flag is set\n");
    }
    if(MOS_6502->registers.AC == user_value){
        //set_zero_flag();
        printf("Zero flag is set\n");
    }

}

// void set_flag(struct MOS_6502 accumulator, uchar8_t user_value){
//     /*
//     C	Carry Flag	Set if A >= M
//     Z	Zero Flag	Set if A = M
//     */
//    printf("Something\n");

// };

/*
CMP - 193-221 - 19 bytes - opcode - C9 for immeadiate
Compare Memory with Accumulator

A - M
N	Z	C	I	D	V
+	+	+	-	-	-

if(result & flag_negative_bit)

*/