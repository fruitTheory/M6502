#include "ppu.h"
#include "M6502_flags.h"
#include <stdlib.h>
#include <stdio.h>
#include "M6502.h"


//uchar8_t vblank = 0;
uchar8_t memory_latch = 0x00;
uchar8_t latch_set = 0;
ushort16_t stored_latch_address = 0x0000;
ushort16_t offset = 0;


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


// parse and store into 2D array 64x4 x-t-a-y - pos X & Y, tile index, attrib
void parse_oam(struct M6502* computer){
    uchar8_t oam_data[64][4] = {0};
    int num = 0;
    for(int a = 0; a < 64; a++){
        for(int b = 0; b < 4; b++){
            oam_data[a][b] = OAM_address[num];
            num++;
        }
    }
    // loops probably needed
    for(int x = 0; x < 64; x++);
    for(int y = 0; y < 4; y++);
    // solve for byte 0 and 3
    ushort16_t y_pos = oam_data[0][0]; // sprite y position
    ushort16_t x_pos = oam_data[0][3]; // sprite x position

    // below solve byte 1 - tile index
    ushort16_t tile_index_start = oam_data[0][1] * 16; // tile address to start at

    // below solve byte 2 - palette index
    // uchar8_t bit0 = check_bit(0, oam_data[0][2]);
    // uchar8_t bit1 = check_bit(1, oam_data[0][2]);

    uchar8_t bit0 = check_bit(0, 0x01); // could also 0x01 & 1
    uchar8_t bit1 = check_bit(1, 0x03); // could also (0x3 >> 1) & 1
    uchar8_t palette_result = (bit1 << 1) + bit0;

    puts("");
    printf("bit0 result: %i\n", bit0);
    printf("bit1 result: %i\n", bit1);
    printf("palette result: %i\n", palette_result);

    // Above checks lowest bits and gets palette result 0-3 
    /*
    Byte 0: Y position 
    Byte 1: Tile index
    Byte 2: Attributes (lower 2 bits combine to pick sprite palette 00 01 10 11)(0 1 2 3)
    Byte 3: X position
    */
}

// parse and store into 2D array 256x16 - 256 patterns 16 bytes each
void parse_patterns(struct M6502* computer){
    uchar8_t pattern_data[256][16] = {0};
    int num = 0;
    for(int a = 0; a < 256; a++){
        for(int b = 0; b < 16; b++){
            pattern_data[a][b] = PPU_address[num];
            num++;
        }
    }
}

// parse and store into 2D array for each 16 byte palette - bg[4x4] and sprite[4x4] 
void parse_palettes(struct M6502* computer){
    uchar8_t background_palette[4][4] = {0};
    int num = 0;
    for(int a = 0; a < 4; a++){
        for(int b = 0; b < 4; b++){
            background_palette[a][b] = PPU_address[palette_start + num];
            num++;
        }
    }
    uchar8_t sprite_palette[4][4] = {0};
    num = 0; // reset
    for(int a = 0; a < 4; a++){
        for(int b = 0; b < 4; b++){
            sprite_palette[a][b] = PPU_address[(palette_start + 16) + num];
            num++;
        }
    }
}


/*
4KB pattern table, two 2KB tables
4096 bytes table
16 bytes per pattern
256 patterns total - 4096/16 
*/
/*
  .byte $6c, $00, $00, $6c  ; this data represents sprite attributes
  .byte $6c, $01, $00, $76  ; OAM info on where sprites are positioned
  .byte $6c, $02, $00, $80  ; y position, index of tile, attribs 0-7b, x position
  .byte $6c, $02, $00, $8A
  .byte $6c, $03, $00, $94
*/

/*
my NES list to get graphics working

The PPU takes CHR data 2 bytes at a time and ORs them to represent an 8 pixel row defining shape
It groups the bits to determine color index for example:
0x11 - 0001 0001
0x01 - 0000 1001
                 - color indx  - 00 00 00 01 10 00 00 11
                 - palette     - 0  0  0  1  2  0  0  3   - palette index 0-3 per shape pixel 
0x19 - 0001 1001 - shape

Nametable points to address of a pattern start so each byte on nametable represents an address

*/

/*
    // puts("");
    // for(int a = 0; a < 4; a++){
    //     puts("");
    //     for(int b = 0; b < 16; b++)
    //         printf("pattern data[%i] %02X \n", b, pattern_data[a][b]);
    // }
*/