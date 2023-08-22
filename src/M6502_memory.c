#include "config.h"
#include "M6502_memory.h"
#include <assert.h>
#include "M6502.h"


void M6502_memory_inbounds(ushort16_t index){
    assert(index > 0 && index < max_memory);
}

void set_memory(M6502_memory* memory, ushort16_t location, ushort16_t value){
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


