#pragma once

#include "config.h"

// OAM (Object Attribute Memory) - internal memory that contains a list of 64 4-byte sprites
typedef struct
{
    uchar8_t CTRL; // $2000 - PPU control register
    uchar8_t MASK; // $2001 - PPU mask register
    uchar8_t STATUS; // $2002 - PPU status register
    uchar8_t OAM_ADDR; // $2003 - OAM address read/write
    uchar8_t OAM_DATA; // $2004 - OAM data read/write
    uchar8_t SCROLL; // $2005 - PPU scrolling position register
    uchar8_t ADDR; // $2006 - PPU address register
    uchar8_t DATA; // $2007 - PPU data port
    uchar8_t OAM_DMA; // $4014 - DMA register (high byte)

}PPU_registers;

typedef struct
{
    uchar8_t address[ppu_max_memory]; // 16kb pattern tables, name tables, attribute tables
    
}PPU_memory;

typedef struct{
    PPU_registers registers;
    PPU_memory memory;
    
}NES_ppu;

/*
PPU CTRL: bits = VPHB SINN
NMI enable (V), PPU master/slave (P), sprite height (H), background tile select (B),
sprite tile select (S), increment mode (I), nametable select (NN)

PPU MASK: bits = BGRs bMmG
color emphasis (BGR), sprite enable (s), background enable (b), 
sprite left column enable (M), background left column enable (m), greyscale (G)

PPUSTATUS: bits = VSO- ----
vblank (V), sprite 0 hit (S), sprite overflow (O); read resets write pair for $2005/$2006
*/

/*
PPU can address 64kb memory like cpu
Only has 16kb RAM (14-bit) address space
Only accessed by PPU or by CPU through $2006-$2007 registers

PPU internally contains 256 bytes of memory known as Object Attribute Memory which determines how sprites are rendered
The CPU can manipulate this memory through memory mapped registers at..
OAMADDR ($2003), OAMDATA ($2004), and OAMDMA ($4014)

OAM can be viewed as an array with 64 entries
Each entry has 4 bytes: the sprite Y coordinate, the sprite tile number,
the sprite attribute, and the sprite X coordinate

Pattern Tables: (Sprites) - Normally mapped by the cartridge to CHR-ROM or CHR-RAM, often with bank switching
$0000-$0FFF (0-4095): Pattern Table 0 (4KB)
$1000-$1FFF (4096-8191): Pattern Table 1 (4KB)

Name Tables and Attributes: (Background Layout) - 2kB NES internal VRAM, nametables mirroring controlled by cartridge
Name table mirroring affects what is shown past the right and bottom edges of current nametable
$2000-$23BF (8192-9151): Name Table 0 
$23C0-$23FF (9152-9215): Attribute Table 0
$2400-$27BF (9216-10175): Name Table 1 (mirrors)
$27C0-$27FF (10176-10239): Attribute Table 1
$2800-$2BBF (10240-11199): Name Table 2 
$2BC0-$2BFF (11200-11263): Attribute Table 2 
$2C00-$2FBF (11264-12223): Name Table 3 
$2FC0-$2FFF (12224-12287): Attribute Table 3

$3000-$3EFF (12288-16127): Mirror of $2000-$2EFF - negligible 

Palettes: (Colours) - not configurable, always mapped to the internal palette control
$3F00-$3F0F (16128-16143): Image Palette - 15 bytes
$3F10-$3F1F (16144-16159): Sprite Palette - 15 bytes
$3F20-$3FFF (16160-16383): Mirrors of $3F00-$3F1F

Note: In a basic NES without additional cartridge hardware, there's 2KB of VRAM for nametables
4 nametables are typically mirrored (Table 2 and 3 might mirror Table 0 and 1)


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