#include "config.h"
#include "M6502.h"
#include "M6502_memory.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

static void cpu_address_inbounds(ushort16_t address){
    assert(address >= 0 && address <= cpu_max_address);
}

// sets byte at memory address
void cpu_set_memory(struct M6502* computer, ushort16_t address, uchar8_t value){

    cpu_address_inbounds(address);
    CPU_address[address] = value;
}

// get bytes at memory address
uchar8_t cpu_get_byte(struct M6502* computer, ushort16_t address){

    cpu_address_inbounds(address);
    return CPU_address[address];
}
// get two bytes in little endian order - increments program counter +1
// increment should always be true unless being used abstractly for indirect/pointers
ushort16_t cpu_get_word(struct M6502* computer, ushort16_t address, uchar8_t increment){
    uchar8_t lo_byte = cpu_get_byte(computer, address);
    uchar8_t hi_byte = cpu_get_byte(computer, address+1);
    if(increment == increment_true) PC_increment(computer);
    // Shifting bits left takes 8 bit -> 16bit, and | merges high byte with low bute
    return hi_byte << 8 | lo_byte; 
}

// store the program into load program address 0x0200
void cpu_store_program(struct M6502* computer, uchar8_t* file, size_t program_size){
    assert(stack_end+program_size < cpu_max_address);
    // copy file into the program load address
    // using & provides the address of the element at this position
    memcpy(&CPU_address[NES_initial_load], file, program_size);

    // Set program counter to a default load address
    program_counter = NES_initial_load; // this is ok for now but need to set to reset vector init

    // If contains NES header offset program counter 16 bytes
    if(NES_header(computer)) program_counter += 16;

    // //print all bytes of file
    // for(int i = 0; i < (program_size); i++){
    //     printf("value at memory address %04X: %02X\n", program_counter+i, CPU_address[NES_initial_load+i]);
    // }
}

// Checks for NES header
bool NES_header(struct M6502* computer){
    char8_t arr[4]; // +1 for null terminator
    for(int i = 0; i < 3; i++){
        arr[i] = cpu_get_byte(computer, NES_initial_load+i);
    }
    // printf("bytes: %c%c%c\n", arr[0],arr[1],arr[2]);
    // compare char array to 'NES' if strings return same then NES true else false
    bool is_nes;
    is_nes = strcmp(arr, "NES") == 0 ? true:false;
    return is_nes;
}

/*
Dealing with iNES headers
Bytes	Description
0-3	Constant $4E $45 $53 $1A (ASCII "NES" followed by MS-DOS end-of-file)
4	Size of PRG ROM in 16 KB units - ex 
5	Size of CHR ROM in 8 KB units (value 0 means the board uses CHR RAM)
6	Flags 6 – Mapper, mirroring, battery, trainer
7	Flags 7 – Mapper, VS/Playchoice, NES 2.0
8	Flags 8 – PRG-RAM size (rarely used extension)
9	Flags 9 – TV system (rarely used extension)
10	Flags 10 – TV system, PRG-RAM presence (unofficial, rarely used extension)
11-15	Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)

Still need to parse the header for this info
*/