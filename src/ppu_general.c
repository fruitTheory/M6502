// #include "M6502.h"
// #include "ppu.h"
#include "ppu_general.h"
#include "M6502_flags.h"
#include "load_binary.h"
#include <stdlib.h>
#include <stdio.h>

uchar8_t memory_latch = 0x00;
uchar8_t latch_set = 0;
ushort16_t stored_latch_address = 0x0000;
ushort16_t offset = 0;

uchar8_t get_pattern(){

    uchar8_t pixel_row[8];

    uchar8_t input = 0x01;
    uchar8_t bit;
    uchar8_t i = 0;

    for(char8_t b = 7; b >= 0; b--){
        bit = check_bit(b, input);
        pixel_row[i] = bit;
        i++;
    }

    for(uchar8_t b = 0; b < 8; b++){
        printf("%i", pixel_row[b]);
        if(b == 3) printf(" ");
    }
    puts("");

    return 0;

}

void PPU_register_handler(struct M6502* computer, ushort16_t address, uchar8_t value, char8_t rw){
    switch(address)
    {
        case 0x2000: // PPU ctrl
            break;
        case 0x2001: // PPU mask
            break;
        case 0x2002: // PPU status
            clear_bit(7, &PPU_status);
            break;
        case 0x2003: // PPU oam addr
            break;
        case 0x2004: // PPU oam data
            // use value stored in oam addr register to store input value
            OAM_address[PPU_oam_addr] = value;
            if(rw == 1) // if write
                CPU_address[0x2003] += 1; // increment address at oam addr register
            break;
        case 0x2005: // PPU scroll
            break;
        case 0x2006: // PPU addr
            // check if latch mem is not set, if already set make full adress, clear latch set and data offset 
            if (!(latch_set == 1)){
                memory_latch = value;
                latch_set = 1;
                printf("latch value: %02X\n", memory_latch);
            }else{ stored_latch_address = ((memory_latch << 8) | value); latch_set = 0; offset = 0;}
            printf("latch address: %04X\n", stored_latch_address);
            break;
        case 0x2007: // PPU data
            if(WRITE){ // STA - 8D
                //puts("2007 call");
                printf("WRITE: %02X\n", value);
                PPU_address[stored_latch_address+offset] = value;
                printf("OFFSET: %i\n", offset);
                offset++;
            }
            break;
        default:
            break;
    }
}

void test_prog(struct M6502* computer){

    const char8_t* path = "C:/Programming/vscode/C/M6502/asm/nes_hello/tiles.chr";
    uchar8_t* program = load_program_temporary(path);
    //print_program_info_temporary(path);
    ushort16_t program_size = get_program_size_temporary(path);
    printf("CHR program size: %i bytes\n", program_size);

    // for(int x = 0; x < 62; x++){
    //     for(int x = 0; x < 2; x++){
    //         printf("byte: %02X ", program[x]);
    //     }
    // }

    free(program), program = NULL;

    // printf("value zp: %02X\n", PPU_address[0x2000]);

}

/*
Nametabe: 
provides address
go to address get first 2 bytes
get result of first 2 bytes |
store result
8 bits/1 byte stored in nametable[0][0]
go to next addres
do byte1 | byte 2 from there
8 bits/1 byte stored in nametable[0][1]
all the way till nametable[0][31]
then go to that first address + 2 byte offset
same bitwise and store result
8 bits/1 byte stored in nametable[1][0]
8 bits/1 byte stored in nametable[1][1]
8 bits/1 byte stored in nametable[1[2]
all the way till nametable[1][31]
and again 30 times for each row unitl [29][31]

theres 16 bytes to make 8x8
16/2 = 8 bytes for each name table index 8*32=256 pix
so 8 iterations per adress loop

now we have a map of numbers, nametable [30][32]
loop through row first then column
extract the bits via check bit and store them into a screen array
screen [240][256]
now we have a screen map of where a pixel needs to be drawn
for nametable it gives a row of 32 byte adreeses so we loop through those 32 byte adresses 8 times
since for each eaddress section there 16 bytes and we operate on pairs of 2 each time
this gives 8x8 pixel rows for row 1 of 30 of the nametables map of screen

now checking for bit hits = if hit, write pixel
so index [0 of 240][0 of 255]
if(i[j][k] == 1) SDL_RenderDrawRect(renderer, &pixel); 

after that we'll need a secondary screen map of stored color indexes
which can be done all the same except instead OR result
we take the 2 bytes to be compared and check first of each least sig bit
so 0000 001[0] and 0000 000[0]
so use check bit to return a result for each
we'll need to check every bit for result 0000 00[1]0 and 0000 00[0]0
no need to store the pairs do immeadiate calculation then store that 
check bit returns intergral 0 and 1 maybe itoa it "01" strcmp to 01 return 0b01
char8_t bit_pair[3]; // +1 for null terminator
if(strcmp(bit_pair, "00") == 0) return 0b00;
if(strcmp(bit_pair, "01") == 0) return 0b01;
if(strcmp(bit_pair, "10") == 0) return 0b10;
store like nametable but colortable[30][32]


and based on that each will return pairs of 00 01 10 11 - will be 0 1 2 3
calculate the pair then start storing
start into color screen [0][0]
next two bytes compare  [0][1]
again until [0[31] them [1][31] [2][31]


*/

/*
Read Instructions:
LDA (Load Accumulator)
LDX (Load X Register)
LDY (Load Y Register)
EOR (Exclusive OR)
AND (Logical AND)
ORA (Logical OR)
CMP (Compare)
CPX (Compare X Register)
CPY (Compare Y Register)
BIT (Bit Test)

Write Instructions:
STA (Store Accumulator)
STX (Store X Register)
STY (Store Y Register)
*/