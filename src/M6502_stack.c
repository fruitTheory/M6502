#include "M6502.h"
#include "M6502_stack.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void M6502_is_stack_inbounds(struct M6502* computer){
    // throw assertion if stack pointer is not within stack range
    // assert(stack_pointer >= 0 && stack_pointer < stack_max_size);
    //printf("SP: %02X\n", stack_pointer);
    assert(stack_pointer >= stack_start && stack_pointer <= stack_end);
}

void M6502_stack_init(struct M6502* computer){
    memset(stack_addresses, 0, sizeof(stack_addresses));
    // copy stack array to stack start address
    memcpy(&memory_address[stack_start], &stack_addresses, sizeof(stack_addresses));
    // stack for the cpu starts high and decrements down
    stack_pointer = stack_end;
}

// push a value to the stack
void M6502_stack_push(struct M6502* computer, uchar8_t push_value){
    // stack stuff in progress
    M6502_is_stack_inbounds(computer);
    memory_address[stack_pointer] = push_value;
    // Pushing bytes to the stack causes the stack pointer to be decremented
    //printf("Stack pointer: %04X\n", stack_pointer);
    printf("Pushed to Stack: %02X\n", memory_address[stack_pointer]);
    stack_pointer -= 1;
    //printf("Stack pointer: %04X\n", stack_pointer);
}

// pop a value off the stack
uchar8_t M6502_stack_pop(struct M6502* computer){
    // figure out later why this need go first tired
    stack_pointer += 1;
    uchar8_t stack_current = memory_address[stack_pointer];
    // Stack incremented when value popped off - Registers can hold one byte
    printf("Popping: %02X\n", stack_current);
    return stack_current;
}