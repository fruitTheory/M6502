#include "M6502.h"
#include "ppu.h"
#include "M6502_flags.h"

/*
my NES list to get graphics working

- completed parsing NES header

The PPU takes CHR data 2 bytes at a time and ORs them to represent an 8 pixel row defining shape
It groups the bits to determine color index for example:
0x11 - 0001 0001
0x01 - 0000 1001
                 - color indx  - 00 00 00 01 10 00 00 11
                 - palette     - 0  0  0  1  2  0  0  3   - palette index 0-3 per shape pixel 
0x19 - 0001 1001 - shape

Nametable points to address of a pattern start so each byte on nametable represents an address

*/

void temp(){

    struct M6502 computer;
    computer.cpu.memory.address[0x2000]; // ctrl register
    computer.ppu.memory.address[0x05];
}
