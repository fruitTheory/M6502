#pragma once

#include "config.h"

typedef struct
{
    uchar8_t address[ppu_max_address]; // 16kb pattern tables, name tables, attribute tables

}PPU_memory;

typedef struct
{
    uchar8_t address[oam_max_address]; // OAM structure for sprites
    
}PPU_OAM_memory;

typedef struct{
    PPU_memory memory;
    PPU_OAM_memory oam_memory;

}NES_ppu;

struct M6502;

void PPU_register_handler(struct M6502* computer, ushort16_t address, uchar8_t value, char8_t rw);
void parse_oam(struct M6502* computer);
void parse_patterns(struct M6502* computer);
void parse_palettes(struct M6502* computer);
uchar8_t get_palette(struct M6502* computer, uchar8_t attributes);
uchar8_t* get_pattern(struct M6502* computer, uchar8_t pattern_index);

/*
PPU memory layout:

Pattern Tables: (Sprites) (CHR)

$0000-$0FFF (0-4095): Pattern Table 0 (4KB)
$1000-$1FFF (4096-8191): Pattern Table 1 (4KB)

Name Tables: (Backgrounds) (VRAM)
Horizontal Mirroring:

$2000-$23FF (8192-9215): Name Table 0 (960 bytes) - Attribute Table 0 (64 bytes)
$2400-$27FF (9216-10239): Name Table 0 (m) (960 bytes) - Attribute Table 0 (m) (64 bytes)
$2800-$2BFF (10240-11263): Name Table 1 (960 bytes) - Attribute Table 1 (64 bytes)
$2C00-$2FFF (11264-12287): Name Table 1 (m) (960 bytes) - Attribute Table 1 (m) (64 bytes)
$3000-$3EFF (12288-16127): Mirrors of $2000-$2EFF - negligible (3kB)

Vertical Mirroring:

$2000-$23FF (8192-9215): Name Table 0 (960 bytes) - Attribute Table 0 (64 bytes)
$2400-$27FF (9216-10239): Name Table 1 (960 bytes) - Attribute Table 1 (64 bytes)
$2800-$2BFF (10240-11263): Name Table 0 (m) (960 bytes) - Attribute Table 0 (m) (64 bytes)
$2C00-$2FFF (11264-12287): Name Table 1 (m) (960 bytes) - Attribute Table 1 (m) (64 bytes)
$3000-$3EFF (12288-16127): Mirrors of $2000-$2EFF - negligible (3kB)

Palettes:

$3F00-$3F0F (16128-16143): Image Palette - 16 bytes
$3F10-$3F1F (16144-16159): Sprite Palette - 16 bytes
$3F20-$3FFF (16160-16383): Mirrors of $3F00-$3F1F

*/

/*
FLAGS:

PPU CTRL: bits = VPHB SINN | $2000
NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B),
sprite tile select (S), increment mode (I), nametable select (NN)

PPU MASK: bits = BGRs bMmG | $2001
color emphasis (BGR), sprite enable (s), background enable (b), 
sprite left column enable (M), background left column enable (m), greyscale (G)

PPUSTATUS: bits = VSO- ---- | $2002 | bits 7 - 0
vblank (V), sprite 0 hit (S), sprite overflow (O); read/lda resets write pair for $2005/$2006


$2000 - PPUCTRL: Controls various PPU parameters like nametable selection, VRAM address increment, 
sprite size, and interrupt generation
$2001 - PPUMASK: Controls rendering features, enabling or disabling the rendering of sprites, 
backgrounds, and color
$2002 - PPUSTATUS: Provides the vertical blank and sprite hit flags, and reflects the state of the PPU
$2003 - OAMADDR: Specifies the address in OAM (Object Attribute Memory) where data will be written or read
$2004 - OAMDATA: Used to read from or write data to the address specified in OAMADDR
$2005 - PPUSCROLL: Sets the initial horizontal and vertical scroll positions for rendering
$2006 - PPUADDR: Sets the address in VRAM for subsequent reads/writes using PPU_DATA
$2007 - PPU_DATA: Reads from or writes data to the address previously specified in PPUADDR

*/

/*
ADDITIONAL:

Note: In a basic NES without additional cartridge hardware, there's 2KB of VRAM for nametables
nametables are typically mirrored

More on palettes:
$3F00	Universal background color
$3F01-$3F03	Background palette 0
$3F05-$3F07	Background palette 1
$3F09-$3F0B	Background palette 2
$3F0D-$3F0F	Background palette 3
$3F10	Mirror of universal background color
$3F11-$3F13	Sprite palette 0
$3F15-$3F17	Sprite palette 1
$3F19-$3F1B	Sprite palette 2
$3F1D-$3F1F	Sprite palette 3
*/

/*
OAM (Object Attribute Memory) - internal memory that contains a list of 64 4-byte sprites(256 bytes)

In OAM memory each sprite has 4 attributes/bytes
-x & y position, tile index, and other info(palette, priority, etc.)

The Sprite itself is not stored in OAM memory region, they're stored in a Pattern Table VRAM
and they are 16 bytes of data - check into OAM sizes
*/

/*
typedef struct
{
    uchar8_t CTRL; // $2000 - PPU control register - write
    uchar8_t MASK; // $2001 - PPU mask register - write
    uchar8_t STATUS; // $2002 - PPU status register - write
    uchar8_t OAM_ADDR; // $2003 - OAM address - read/write
    uchar8_t OAM_DATA; // $2004 - OAM data read/write - read/write
    uchar8_t SCROLL; // $2005 - PPU scrolling position register
    uchar8_t ADDR; // $2006 - PPU address register - write x2
    uchar8_t DATA; // $2007 - PPU data port - read/write
    uchar8_t OAM_DMA; // $4014 - DMA register (high byte) - write

}PPU_registers;
*/

/*
ADDITIONAL:

PPU ctrl info:

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

