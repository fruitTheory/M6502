#include "config.h"
#include "MOS_6502_memory.h"
#include <assert.h>
#include "MOS_6502.h"


// note this was previously static 
void MOS_6502_memory_inbounds(int index){
    assert(index > 0 && index < max_memory);
}

void set_memory(int index){
    int temp_index = 1;
    MOS_6502_memory_inbounds(temp_index);
}

uchar8_t MOS_6502_memory_get(MOS_6502_memory* memory, int location){
    
    MOS_6502_memory_inbounds(location);
    return memory->address[location];

}

ushort16_t MOS_6502_memory_get_short(MOS_6502_memory* memory, int location){
    uchar8_t byte1 = MOS_6502_memory_get(memory, location);
    uchar8_t byte2 = MOS_6502_memory_get(memory, location+1);
    printf("byte1 = %i byte2 = %i\n", byte1, byte2);

    // A char is 8 bits wide, shifting 8 bits left leaves all 0
    return byte1 << 8 | byte2;
}


// One way to get the opcodes from file

// uchar8_t MOS_6502_memory_get(MOS_6502_memory* memory, int location){
    
//     MOS_6502_memory_inbounds(location);
//     return memory->address[location];

// }

// ushort16_t MOS_6502_memory_get_short(MOS_6502_memory* memory, int location){
//     uchar8_t byte1 = MOS_6502_memory_get(memory, location);
//     uchar8_t byte2 = MOS_6502_memory_get(memory, location+1);
//     return byte1 << 8 | byte2;
// }

// int temp(){
//     struct MOS_6502 MOS_6502;
//     ushort16_t opcode = MOS_6502_memory_get_short(&MOS_6502.memory, MOS_6502.registers.PC);
// }



