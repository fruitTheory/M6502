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
Only has 16kb RAM

Pattern Tables:
$0000-$0FFF (0-4095 in decimal): Pattern Table 0 (4KB)
$1000-$1FFF (4096-8191 in decimal): Pattern Table 1 (4KB)

Name Tables and Attributes:
These are for background tiles

$2000-$23BF (8192-9151 in decimal): Name Table 0 (960 bytes)
$23C0-$23FF (9152-9215 in decimal): Attribute Table 0 (64 bytes)

$2400-$27BF (9216-10175 in decimal): Name Table 1 (960 bytes)
$27C0-$27FF (10176-10239 in decimal): Attribute Table 1 (64 bytes)

$2800-$2BBF (10240-11199 in decimal): Name Table 2 (960 bytes)
$2BC0-$2BFF (11200-11263 in decimal): Attribute Table 2 (64 bytes)

$2C00-$2FBF (11264-12223 in decimal): Name Table 3 (960 bytes)
$2FC0-$2FFF (12224-12287 in decimal): Attribute Table 3 (64 bytes)

Note: In a basic NES without additional cartridge hardware, there's only 2KB of VRAM for nametables,
so these 4 nametables are typically mirrored (e.g., Tables 2 and 3 might mirror Tables 0 and 1, respectively)

Palettes:
$3F00-$3F0F (16128-16143 in decimal): Image Palette
$3F10-$3F1F (16144-16159 in decimal): Sprite Palette

Mirrors:
$3000-$3EFF (12288-16127 in decimal): Mirror of $2000-$2EFF
$3F20-$3FFF (16160-16383 in decimal): Mirrors of $3F00-$3F1F

*/