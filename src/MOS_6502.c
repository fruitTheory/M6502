#include "MOS_6502.h"
#include <memory.h>
#include <assert.h>
#include <stdio.h>

// initialize the processor
void MOS_6502_init(struct MOS_6502* MOS_6502){
    memset(MOS_6502, 0, sizeof(&MOS_6502));
    MOS_6502_stack_init(MOS_6502);
}

// store the program into memory
void program_file_store(struct MOS_6502* MOS_6502, uchar8_t* file, size_t program_size){

    assert(stack_end+program_size < max_memory);
    // copy buffer[size] into the program load location
    memcpy(&MOS_6502->memory.address[program_initial_load], file, program_size);
    MOS_6502->registers.PC = program_initial_load;

    // print all bytes of file
    for(int i = 0; i < (program_size); i++){
        printf("value at memory address: %02X\n", MOS_6502->memory.address[program_initial_load+i]);
    }
}

void execute_instruction(struct MOS_6502* MOS_6502, ushort16_t opcode){

    switch (opcode)
    {
    case 0x00: // BRK - Force break
        //BRK();
        break;

    case 0x02: // CMP_ABSOLUTE_X
        //LDA_Immeadiate(&MOS_6502);
        break;

    default:
        break;
    }
}

