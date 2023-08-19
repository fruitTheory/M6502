#include "config.h"
#include "MOS_6502_memory.h"
#include <assert.h>
#include "MOS_6502.h"


static void MOS_6502_memory_inbounds(int index){
    assert(index > 0 && index < max_memory);
}

void set_memory(int index){
    int temp_index = 1;
    MOS_6502_memory_inbounds(temp_index);
}

// One way to get the opcodes from file

uchar8_t MOS_6502_memory_get(MOS_6502_memory* memory, int location){
    
    MOS_6502_memory_inbounds(location);
    return memory->address[location];

}

ushort16_t MOS_6502_memory_get_short(MOS_6502_memory* memory, int location){
    uchar8_t byte1 = MOS_6502_memory_get(memory, location);
    uchar8_t byte2 = MOS_6502_memory_get(memory, location+1);
    return byte1 << 8 | byte2;
}

int temp(){
    struct MOS_6502 MOS_6502;
    ushort16_t opcode = MOS_6502_memory_get_short(&MOS_6502.memory, MOS_6502.registers.PC);
}



