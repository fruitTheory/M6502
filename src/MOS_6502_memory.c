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