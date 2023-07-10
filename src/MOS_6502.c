#include "MOS_6502.h"
#include <memory.h>


void MOS_6502_init(struct MOS_6502* MOS_6502){
    memset(MOS_6502, 0, sizeof(struct MOS_6502));
    //memcpy(MOS_6502, 0, sizeof(struct MOS_6502));
}

void M0S_6502_load(struct MOS_6502* MOS_6502, const char* buffer, size_t size){


}

