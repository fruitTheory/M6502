#include "config.h"
#include "MOS_6502_memory.h"
#include <assert.h>
#include "MOS_6502.h"


void MOS_6502_memory_inbounds(ushort16_t index){
    assert(index > 0 && index < max_memory);
}

void set_memory(MOS_6502_memory* memory, ushort16_t location, ushort16_t value){
    int temp_index = 1;
    MOS_6502_memory_inbounds(temp_index);
    memory->address[location] = value;

}

uchar8_t MOS_6502_memory_get(MOS_6502_memory* memory, ushort16_t location){
    
    MOS_6502_memory_inbounds(location);
    return memory->address[location];

}

ushort16_t MOS_6502_memory_get_short(MOS_6502_memory* memory, ushort16_t location){
    uchar8_t byte1 = MOS_6502_memory_get(memory, location);
    uchar8_t byte2 = MOS_6502_memory_get(memory, location+1);
    // Shifting bits left takes 8 bit -> 16bit, and | merges high byte
    return byte1 << 8 | byte2; 
}
