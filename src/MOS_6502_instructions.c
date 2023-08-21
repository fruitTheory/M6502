#include "MOS_6502_instructions.h"


void LDA_Immeadiate(struct MOS_6502* MOS_6502, uchar8_t user_value){
    MOS_6502->registers.AC = user_value;
}

void LDX_Zero_Page(struct MOS_6502* MOS_6502, uchar8_t user_value){
    MOS_6502->registers.AC = user_value;
}

void LDX_Zero_Page_X(struct MOS_6502* MOS_6502, uchar8_t user_value){
    MOS_6502->registers.AC = user_value + MOS_6502->registers.X;
}

void LDA_Absolute(struct MOS_6502* MOS_6502, ushort16_t user_value);


/*
LDA(mos6502, int user_value){
    if immeadiate
    mos6502.registers.AC = user_value(19);

    if LDA zeropage(0xA5)
    mos6502.registers.AC = user_value(0x00FF);
    Example: LDA $50 loads the value at memory address 0x0050.

    if LDA absolute(0xAD)
    mos6502.registers.AC = user_value(0xFFFF);
}
*/