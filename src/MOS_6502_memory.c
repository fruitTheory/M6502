#include "config.h"
#include "MOS_6502_memory.h"
#include <assert.h>


static void MOS_6502_memory_inbounds(int index){
    assert(index > 0 && index < max_memory);
}

void set_memory(int index){
    int temp_index = 1;
    MOS_6502_memory_inbounds(temp_index);
}
ushort16_t get_memory(int index){
    return 0;
}


/*
One way to get the opcodes from file

unsigned short opcode = chip8_memory_get_short(&chip8.memory, chip8.registers.PC);

unsigned short chip8_memory_get_short(struct chip8_memory* memory, int index){
    unsigned char byte1 = chip8_memory_get(memory, index);
    unsigned char byte2 = chip8_memory_get(memory, index+1);
    return byte1 << 8 | byte2;
}

unsigned char chip8_memory_get(struct chip8_memory* memory, int index){
    
    chip8_is_memory_in_bounds(index);
    return memory->memory[index];

}

*/