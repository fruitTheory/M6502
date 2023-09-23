#pragma once

#include "config.h"
#include <stddef.h> // size_t
#include <stdbool.h>

typedef struct
{
    // including RAM, ROM, and memory-mapped I/O
    // each address can hold one byte
    uchar8_t address[cpu_max_address];
}CPU_memory;

struct M6502; // forward declaration so compiler knows struct exists and will be defined later

void memory_init(struct M6502* computer);

void M6502_memory_inbounds(ushort16_t index);
void cpu_set_memory(struct M6502* computer, ushort16_t address, uchar8_t value);

uchar8_t cpu_get_byte(struct M6502* computer, ushort16_t address);
ushort16_t cpu_get_word(struct M6502* computer, ushort16_t address, uchar8_t increment);

void cpu_store_program(struct M6502* computer, uchar8_t* file, size_t program_size);
bool is_NES_header(uchar8_t* file);
void parse_NES_header(struct M6502* computer);

/*
Memory Mapping (nes):

RAM: 2 KB
$0000-$07FF (0-2047): General internal RAM

$0000-$00FF (0-255): zero page - quick addressing
$0100-$01FF (256-511): reserved for stack
$0200-$07FF (512-2047): general purpose RAM
$0800-$1FFF(2048-8191): RAM mirrors

PPU and I/0: 7 KB
$2000-$2007 (8192-8199): Registers used to control the PPU. They're memory-mapped, so the CPU interacts with the PPU by reading from and writing to these addresses.
$2008-$3FFF (8200-16383): PPU Mirrors - Each 8 bytes of PPU $2000-$2007

APU and I/O Registers: 31 bytes
$4000-$401F (16384-16415): Used to control the APU and other I/O functions

Cartridge Space: 47 KB
$4020-$FFFF (16416-65535): includes ROM and possibly RAM, depending on the cartridge

$4020-$5FFF (16416-24575): cartridge RAM or extra hardware in cartridge
$6000-$7FFF (24576-32767): Often used for battery-backed Save RAM
$8000-$FFF9(32768-65529): PRG-ROM, where game program code is usually located

System Vectors: 6 bytes
$FFFA-$FFFF (65530-65535)
($FFFA/B): Non-maskable interrupt handler 
($FFFC/D): Power-on/Reset
($FFFE/F): BRK/interrupt request handler

*/