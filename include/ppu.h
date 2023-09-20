#pragma once

#include "config.h"

// OAM (Object Attribute Memory) - internal memory that contains a list of 64 4-byte sprites
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

typedef struct
{
    uchar8_t address[ppu_max_address]; // 16kb pattern tables, name tables, attribute tables
    
}PPU_memory;

typedef struct{
    PPU_registers registers;
    PPU_memory memory;
    
}NES_ppu;

void PPU_CTRL(struct M6502* computer);

// // just to note tiles and pixels need to be at different scales, realistically, but can abstract it probably
// uchar8_t tile[8][8]; // tile size in pixels  - pixels
// uchar8_t screen_tiles[32][30]; // tiles on screen - tiles
// uchar8_t screen_pixels[256][240]; // pixels on screen - pixels
// uchar8_t nametable[960]; // tiles(bytes) - total screen - 960 bytes
// uchar8_t attribute_table[8][8]; // One byte for 4 tiles - holds palette data - 64 bytes
// // palette + background = 1024 bytes needed in VRAM - 2k vram so can hold 2 screens in total - 2048 bytes

/*
OAM is a space of memory of 256 bytes and can store 64 sprite(attributes) - 4 bytes is cost of each sprite
In OAM memory each sprite has 4 attributes(bytes) - x & y position, tile index, and other info(palette, priority, etc.)

The Sprite itself is not stored in OAM memory region, they're stored in a Pattern Tables within VRAM
And a Sprite is 16 bytes of data, 8x8 bytes
*/

/*
Finding CHR data in ROM - vary based on header mapping, address is $8010 if 2 16k prg-rom banks + 16byte header
Finding Nametables/Palettes - The programmer makes a routine to store the table data in the ppu VRAM
*/

/*
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