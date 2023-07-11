#include "MOS_6502.h"
#include <memory.h>
#include <assert.h>


void MOS_6502_init(struct MOS_6502* MOS_6502){
    memset(MOS_6502, 0, sizeof(struct MOS_6502));
    
    //copy something into memory
    //memcpy(MOS_6502->memory.memory, something, sizeof(something));
}


// think this needs to be put in load_binary can rename to mos_6502_load_program/binary
// OR WE NEED a function that outputs the size of the file inputted

void M0S_6502_program_load(struct MOS_6502* MOS_6502, const char* buffer, size_t program_size){
    // program is less than max memory or throw assertion
    assert(stack_end+program_size < max_memory);
    // copy 
    memcpy(MOS_6502->memory.memory[program_initial_load], buffer, program_size);

    // initialize the program counter to
    MOS_6502->registers.PC = program_initial_load;
}
