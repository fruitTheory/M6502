#include "config.h"
#include "M6502_memory.h"
#include <assert.h>
#include "M6502.h"
#include <string.h>
#include <stdio.h>

static void M6502_address_inbounds(ushort16_t index){
    assert(index > 0 && index < max_address);
}
// sets byte at memory address
void M6502_set_memory(struct M6502* computer, ushort16_t address, uchar8_t value){

    M6502_address_inbounds(address);
    memory_address[address] = value;
}

// increment program counter by + 1
void PC_increment(struct M6502* computer){
    program_counter += 1;
    instruction_byte_count += 1;
}

// get bytes at memory address
uchar8_t M6502_get_byte(struct M6502* computer, ushort16_t address){

    M6502_address_inbounds(address);
    return memory_address[address];
}
// get two bytes in little endian order - increments program counter +1
// increment should always be true unless being used outside of program counter (abstracted)
ushort16_t M6502_get_word(struct M6502* computer, ushort16_t address, uchar8_t increment){
    uchar8_t lo_byte = M6502_get_byte(computer, address);
    uchar8_t hi_byte = M6502_get_byte(computer, address+1);
    if(increment == increment_true) PC_increment(computer);
    // Shifting bits left takes 8 bit -> 16bit, and | merges high byte with low bute
    return hi_byte << 8 | lo_byte; 
}

// store the program into load program address 0x0200
void M6502_store_program(struct M6502* computer, uchar8_t* file, size_t program_size){
    assert(stack_end+program_size < max_address);
    // copy file into the program load address
    memcpy(&memory_address[program_initial_load], file, program_size);
    // using & provides the address of the element at the position [program_init_load]

    // Set program counter to default load address and print all bytes onward until reaching program size
    program_counter = program_initial_load;
    // print all bytes of file
    for(int i = 0; i < (program_size); i++){
        printf("value at memory address %04X: %02X\n", program_counter+i, memory_address[program_initial_load+i]);
    }
}
