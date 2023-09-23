#include "config.h"
#include "M6502.h"
#include "M6502_memory.h"
#include "ppu.h"
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

    // If contains NES header, parse header, and clean that memory
    if(is_NES_header(file)){
        // copy file into the program load address
        // using & provides the address of the element at this position
        memcpy(&CPU_address[NES_initial_load], file, program_size);

        // Set program counter to a default load address
        program_counter = NES_initial_load;

        // parse header and clear that memory
        parse_NES_header(computer);
        memset(&CPU_address[NES_initial_load], 0, header_size);

    } else memcpy(&CPU_address[program_initial_load], file, program_size);
}

// Checks file for NES header
bool is_NES_header(uchar8_t* file){
    char8_t arr[4]; // +1 null terminator for strcmp
    for(int i = 0; i < 3; i++) {arr[i] = file[i];}
    arr[3] = '\0'; // char literal

    bool is_nes; // compare cstring to 'NES' 
    is_nes = strcmp(arr, "NES") == 0 ? true:false;
    return is_nes;
}

// Parse NES header and input CHR data into PPU memory, also offset program counter by 16 bytes
void parse_NES_header(struct M6502* computer){
    char8_t header_byte[16];
    
    // loop through 16 header bytes
    for(int i = 0; i < 16; i++){
        header_byte[i] = cpu_get_byte(computer, NES_initial_load+i);

        if(i == 4){} // size of PRG rom in 16KB units
        if(i== 5){  // size of CHR rom in 8KB units

            // finds chr data in cartridge.nes, which is after the PRG data + header data
            ushort16_t CHR_cartidge_address = (PRG_rom_unit * header_byte[4]) + header_size;
            uchar8_t CHR_data[CHR_rom_unit]; // chr data storage

            // store whats at cpu addresses initial load address, offset by char address and inc by j
            for(int j=0; j <  CHR_rom_unit; j++)
                CHR_data[j] = CPU_address[NES_initial_load + CHR_cartidge_address + j];

            // copy data directly into PPU, starting at address 0x00
            memcpy(&PPU_address[0x0000], CHR_data, (CHR_rom_unit * header_byte[5]));
        }
        if(i== 6){} // ROM's mirroring type 
        if(i== 7){} // ROM's mapper
    }
    // offset by 16 so counter starts at 0x8000 where program code is
    program_counter += 16;
}

/*
Dealing with iNES headers
Bytes	Description

0-3	Constant $4E $45 $53 $1A (ASCII "NES" followed by MS-DOS end-of-file)
4	Size of PRG ROM in 16 KB units - ex 
5	Size of CHR ROM in 8 KB units (value 0 means the board uses CHR RAM)
6	Flags 6 – Mirroring type, if use battery-backed RAM, if use trainer
7	Flags 7 – ROMs Mapper, VS/Playchoice, NES 2.0

8	Flags 8 – PRG-RAM size (rarely used extension)
9	Flags 9 – TV system (rarely used extension) typically 0 
10	Flags 10 – TV system (PRG-RAM presence) typically 0 
11-15	Unused padding (should be filled with zero)

*/