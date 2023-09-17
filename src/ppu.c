#include "M6502.h"
#include "ppu.h"
#include "M6502_flags.h"

void temp(){

    struct M6502 computer;
    computer.ppu.registers.CTRL;
    computer.ppu.memory.address[0x05];

}

void PPU_CTRL(struct M6502* computer){ // $2000
    
    PPU_ctrl; // $2000 computer->cpu.memory.adress[0x2000] - memory mapped
    
    /*
    7 bit 0
    VPHB SINN

    NN - bit 0, 1
    Nametame address per bit - believe this is 0 1 2 3 in binary 
    (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)

    bit 0 = 1: Add 256 to the X scroll position
    bit 1 = 1: Add 240 to the Y scroll position
    when you reach the end of a nametable, you must switch 
    to the next one, hence, changing the nametable address

    I - bit 2
    Increment VRAM address per CPU read/write of PPUDATA
    (0: add 1, going across; 1: add 32, going down)

    S - bit 3
    Sprite pattern table address for 8x8 sprites
    (0: $0000; 1: $1000; ignored in 8x16 mode)

    B - bit 4
    Background pattern table address (0: $0000; 1: $1000)

    H - bit 5
    Sprite size (0: 8x8 pixels; 1: 8x16 pixels – check out PPU_OAM byte 1)

    P - bit 6
    PPU master/slave select
    (0: read backdrop from EXT pins; 1: output color on EXT pins)

    V - bit 7
    Vertical blanking - Generate an NMI at the start of the
    vblank interval (0: off; 1: on)

    */

   /*
    Additional
    it is important to read $2002 immediately
    before writing $2000 to clear the vblank flag
   */
    
}

// void PPU_MASK(struct M6502* computer){ // $2001
//     PPU_mask;
// }

// void PPU_STATUS(){
    
// }