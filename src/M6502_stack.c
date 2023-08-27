#include "M6502_stack.h"
#include "M6502.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void M6502_is_stack_inbounds(struct M6502* computer){
    // throw assertion if stack pointer is not greater than 0 and less than max size
    assert(computer->registers.SP >= 0 && computer->registers.SP < stack_max_size);
}

void M6502_stack_init(struct M6502* computer){
    memset(computer->stack.stack_address, 0, sizeof(computer->stack.stack_address));
    memcpy(&computer->memory.address[stack_start], &computer->stack.stack_address, sizeof(computer->stack.stack_address));
    // printf("stack after copy: %i\n", computer->memory.address[stack_start-1]);
    // printf("stack after copy: %i\n", computer->memory.address[stack_start]);
    // printf("stack after copy: %i\n", computer->memory.address[stack_start+1]);
}

void M6502_stack_push(struct M6502* computer, ushort16_t val){
    
    M6502_is_stack_inbounds(computer);
    computer->stack.stack_address[computer->registers.SP] = val;
    // Pushing bytes to the stack causes the stack pointer to be decremented 
    computer->registers.SP -= 1;
    printf("Stack Pointer = %i\n", computer->registers.SP);
}

void M6502_stack_pop(struct M6502* computer, ushort16_t val){

    M6502_is_stack_inbounds(computer); 
    computer->stack.stack_address[computer->registers.SP] = val;
    // Stack is incremented when value popped off - Registers can only hold on byte
    computer->registers.SP += 1;
    printf("Stack Pointer = %i\n", computer->registers.SP);
}