#include "M6502_stack.h"
#include "M6502.h"
#include <assert.h>
#include <stdio.h>


static void M6502_stack_inbounds(struct M6502* M6502){
    // throw assertion if stack pointer is not greater than 0 and less than max size
    assert(M6502->registers.SP >= 0 && M6502->registers.SP < stack_max_size);
}

void M6502_stack_init(struct M6502* M6502){
    // be careful this converts stack end from int to char 511 to 255
    M6502->registers.SP = stack_end;
}

void M6502_stack_push(struct M6502* M6502, ushort16_t val){
    
    M6502_stack_inbounds(M6502);
    M6502->stack.stack[M6502->registers.SP] = val;
    // Pushing bytes to the stack causes the stack pointer to be decremented 
    M6502->registers.SP -= 1;
    printf("Stack Pointer = %i\n", M6502->registers.SP);
}

void M6502_stack_pop(struct M6502* M6502, ushort16_t val){

    M6502_stack_inbounds(M6502); 
    M6502->stack.stack[M6502->registers.SP] = val;
    // Stack is incremented when value popped off - Registers can only hold on byte
    M6502->registers.SP += 1;
    printf("Stack Pointer = %i\n", M6502->registers.SP);
}