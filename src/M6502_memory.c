#include "config.h"
#include "M6502_memory.h"
#include <assert.h>
#include "M6502.h"
#include <string.h>
#include <stdio.h>


void M6502_memory_inbounds(ushort16_t index){
    assert(index > 0 && index < max_memory);
}
// set byte at memory location
void M6502_set_memory(struct M6502* computer, ushort16_t location, uchar8_t value){

    M6502_memory_inbounds(location);
    computer->memory.address[location] = value;
}
// get byte at location
uchar8_t M6502_memory_get_byte(struct M6502* computer, ushort16_t location){
    
    M6502_memory_inbounds(location);
    return computer->memory.address[location];
}
// get two bytes in little endian order
ushort16_t M6502_memory_get_word(struct M6502* computer, ushort16_t location){
    uchar8_t lo_byte = M6502_memory_get_byte(computer, location+1);
    uchar8_t hi_byte = M6502_memory_get_byte(computer, location+2);
    // Shifting bits left takes 8 bit -> 16bit, and | merges high byte with low bute
    return hi_byte << 8 | lo_byte; 
}

// store the program into load program address 0x0200
void M6502_store_program(struct M6502* computer, uchar8_t* file, size_t program_size){
    assert(stack_end+program_size < max_memory);
    // copy file into the program load location
    memcpy(&computer->memory.address[program_initial_load], file, program_size);

    // Set program counter to default load address and print all bytes onward until reaching program size
    computer->registers.PC = program_initial_load;
    // print all bytes of file
    for(int i = 0; i < (program_size); i++){
        printf("value at memory address %04X: %02X\n", computer->registers.PC+i, computer->memory.address[program_initial_load+i]);
    }
}
