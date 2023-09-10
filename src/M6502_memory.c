#include "config.h"
#include "M6502.h"
#include "M6502_memory.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

static void M6502_address_inbounds(ushort16_t address){
    assert(address >= 0 && address <= max_address);
}

// sets byte at memory address
void M6502_set_memory(struct M6502* computer, ushort16_t address, uchar8_t value){

    M6502_address_inbounds(address);
    memory_address[address] = value;
}

// get bytes at memory address
uchar8_t M6502_get_byte(struct M6502* computer, ushort16_t address){

    M6502_address_inbounds(address);
    return memory_address[address];
}
// get two bytes in little endian order - increments program counter +1
// increment should always be true unless being used abstractly for indirect/pointers
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
    // using & provides the address of the element at this position
    memcpy(&memory_address[NES_initial_load], file, program_size);

    // Set program counter to a default load address
    program_counter = NES_initial_load; // this is ok for now but need to set to reset vector init

    // If contains NES header offset program counter 16 bytes
    if(NES_header(computer)) program_counter += 16;

    // //print all bytes of file
    // for(int i = 0; i < (program_size); i++){
    //     printf("value at memory address %04X: %02X\n", program_counter+i, memory_address[NES_initial_load+i]);
    // }
}

// Checks for NES header
bool NES_header(struct M6502* computer){
    char8_t arr[4]; // +1 for null terminator
    for(int i = 0; i < 3; i++){
        arr[i] = M6502_get_byte(computer, NES_initial_load+i);
    }
    // printf("bytes: %c%c%c\n", arr[0],arr[1],arr[2]);
    // compare char array to 'NES' if strings return same then NES true else false
    bool is_nes;
    is_nes = strcmp(arr, "NES") == 0 ? true:false;
    return is_nes;
}
