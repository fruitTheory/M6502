#include "MOS_6502_stack.h"
#include "MOS_6502.h"
#include <assert.h>
#include <stdio.h>


static void MOS_6502_stack_inbounds(struct MOS_6502* MOS_6502){
    // throw assertion if stack pointer is not greater than 0 and less than max size
    assert(MOS_6502->registers.SP >= 0 && MOS_6502->registers.SP < stack_max_size);
}

void MOS_6502_stack_init(struct MOS_6502* MOS_6502){
    MOS_6502->registers.SP = stack_end;
}

void MOS_6502_stack_push(struct MOS_6502* MOS_6502, ushort16_t val){
    
    MOS_6502_stack_inbounds(MOS_6502);
    MOS_6502->stack.stack[MOS_6502->registers.SP] = val;

    // Pushing bytes to the stack causes the stack pointer to be decremented 
    MOS_6502->registers.SP -= 1;

    printf("Stack Pointer = %i\n", MOS_6502->registers.SP);
}

void MOS_6502_stack_pop(struct MOS_6502* MOS_6502, ushort16_t val){

    MOS_6502_stack_inbounds(MOS_6502); 
    MOS_6502->stack.stack[MOS_6502->registers.SP] = val;

    // Stack is incremented when value popped off - Registers can only hold on byte
    MOS_6502->registers.SP += 1;

    printf("Stack Pointer = %i\n", MOS_6502->registers.SP);

}