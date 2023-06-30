#include "config.h"
#include "MOS_6502_memory.h"


static void MOS_6502_memory_inbounds(int index){
    assert(index > 0 && index < max_memory);
}

void set_memory(){}
unsigned short get_memory(){}