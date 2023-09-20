#include "M6502.h"
#include "M6502_stack.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void M6502_is_stack_inbounds(struct M6502* computer){
    // assertion if stack pointer is not within stack memory range
    //assert(stack_pointer >= stack_start && stack_pointer <= stack_end);
    assert(stack_container[0] >= 0x00 && stack_container[0] <= 0xFF);

}

void M6502_stack_init(struct M6502* computer){
    // set all stack values to 0
    memset(stack_container, 0, sizeof(stack_container));
    // copy stack array to stack start address
    memcpy(&CPU_address[stack_start], &stack_container, sizeof(stack_container));
    // stack for the cpu starts high and decrements down
    stack_pointer = stack_end;
}

// push a value to the stack
void M6502_stack_push(struct M6502* computer, uchar8_t push_value){
    M6502_is_stack_inbounds(computer);
    CPU_address[stack_pointer] = push_value; // set stack position to set value
    printf("Pushed to Stack: %02X\n", CPU_address[stack_pointer]);
    // Pushing bytes to the stack causes the stack pointer to be decremented
    stack_pointer -= 1;
}

// pop a value off the stack
uchar8_t M6502_stack_pop(struct M6502* computer){
    // figure out later why this need go first tired
    stack_pointer += 1;
    uchar8_t stack_current = CPU_address[stack_pointer];
    // Stack incremented when value popped off - Registers can hold one byte
    printf("Pop from Stack: %02X\n", stack_current);
    return stack_current;
}