#include "MOS_6502_stack.h"
#include "MOS_6502.h"
#include <assert.h>
#include <stdio.h>


static void MOS_6502_stack_inbounds(struct MOS_6502* MOS_6502){
    //assert(MOS_6502->registers.SP < stack_max_size);
}

void MOS_6502_stack_push(struct MOS_6502* MOS_6502){
    //MOS_6502->registers.SP--;
    //printf("%i\n", MOS_6502->registers.SP);
    printf("test");
}

void MOS_6502_stack_pop(struct MOS_6502* MOS_6502){
    //MOS_6502->registers.SP++;
    //printf("%i\n", MOS_6502->registers.SP);
    printf("test");
}