#include "config.h"
#include "M6502_memory.h"
#include <assert.h>
#include "M6502.h"
#include <string.h>
#include <stdio.h>


void M6502_memory_inbounds(ushort16_t index){
    assert(index > 0 && index < max_memory);
}

void M6502_set_memory(M6502_memory* memory, ushort16_t location, ushort16_t value){
    int temp_index = 1;
    M6502_memory_inbounds(temp_index);
    memory->address[location] = value;
}

uchar8_t M6502_memory_get(M6502_memory* memory, ushort16_t location){
    
    M6502_memory_inbounds(location);
    return memory->address[location];
}

ushort16_t M6502_memory_get_short(M6502_memory* memory, ushort16_t location){
    uchar8_t byte1 = M6502_memory_get(memory, location);
    uchar8_t byte2 = M6502_memory_get(memory, location+1);
    // Shifting bits left takes 8 bit -> 16bit, and | merges high byte
    return byte1 << 8 | byte2; 
}

// store the program into memory
void M6502_store_program(struct M6502* M6502, uchar8_t* file, size_t program_size){
    assert(stack_end+program_size < max_memory);

    // copy file into the program load location
    memcpy(&M6502->memory.address[program_initial_load], file, program_size);
    M6502->registers.PC = program_initial_load;

    // print all bytes of file
    for(int i = 0; i < (program_size); i++){
        printf("value at memory address %04X: %02X\n", M6502->registers.PC+i, M6502->memory.address[program_initial_load+i]);
    }
}
