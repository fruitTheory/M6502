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

// handles actions on ppu registers
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
uchar8_t** parse_oam(struct M6502* computer){

    // maximum of 64 sprites in oam
    char8_t pos_y[64]; // sprite attribute byte 0
    char8_t pattern_index[64]; // sprite attribute byte 1
    /*
        bit 0, 1 - Determine palette of sprite
        bit 2, 3, 4 - Not implemented
        bit 5 - Priority - 0 in front of background, 1 behind background
        bit 6 - Flip sprite horizontally
        bit 7 - Flip sprite vertically
    */
    char8_t attributes[64];
    char8_t pos_x[64]; // sprite attribute byte 3
    uchar8_t oam_data[64][4] = {0}; // container for each sprites attribute data

    int num = 0;
    for(int a = 0; a < 64; a++){
        for(int b = 0; b < 4; b++){
            // cycle through address linearly, store each sprites 4 byte data 
            oam_data[a][b] = OAM_address[num];
            num++;
        }
    }

    // loop to store sprite specific data
    for(int c = 0; c < 64; c++){
        pos_y[c] = oam_data[c][0];
        pattern_index[c] = oam_data[c][1];
        attributes[c] = oam_data[c][2];
        pos_x[c] = oam_data[c][3];
    }

    // get initial singular pattern
    uchar8_t* pattern = get_pattern(computer, pattern_index[0]);
    uchar8_t pattern_0[8] = {0};
    uchar8_t pattern_1[8] = {0}; // note to use memcpy for larger arrays
    for(int d = 0; d < 8; d++){pattern_0[d] = pattern[d];}
    for(int d = 0; d < 8; d++){pattern_1[d] = pattern[d+8];}
    // above stores first 8 bits and second 8 bits of pattern for later usage
    free(pattern);

    // convert pattern data into shape by using OR of pattern 0 | 1
    uchar8_t shape_bytes[8];
    for(int d = 0; d < 8; d++){shape_bytes[d] = pattern_0[d] | pattern_1[d];}

    // convert resulting shape bytes into binary 8x8 = 64 bits per shape
    uchar8_t shape_binary[64]={0};
    int inc = 0;
    for(int x = 0; x < 8; x++){ // 8 rows
        for(int d = 7; d >= 0; d--){ // 8 bits
            shape_binary[inc] = (shape_bytes[x] >> d) & 1;
            inc++;
        }
    }

    // initialize virtual screen setup variables - add bits to vscreen
    int row = 240; int column = 256;
    //uchar8_t virtual_screen[240][256]={0};
    int index = 0; // current sprite index
    int bit_increment = 0;

    uchar8_t** virtual_screen = create_2D_array(row, column, sizeof(uchar8_t*));
    
    // take shape_binaries put into virtual screen, row by row, based on, x and y coord
    for(int row = 0; row < 8; row++){
        for(int bit = 0; bit < 8; bit++){
            // current sprite index and the relevant value of pos y and x, offset by loop values
            virtual_screen[(pos_y[index] + row)][(pos_x[index] + bit)] = shape_binary[bit_increment];
            bit_increment++; // general bit increment
        }
    }

    uchar8_t palette = get_palette(computer, attributes[0]);
    // color_table = get_color(computer, pattern)
    // map the color to virtual screen based on x and y from sprite data

    return virtual_screen;

}

// returns a dynamically created 2D array of type char
uchar8_t** create_2D_array(int row, int column, size_t type_size){
    // amount of pointers to individual pointers
    uchar8_t** arr = malloc(row * type_size);
    if(arr == NULL){perror("Failed to allocate for rows"); exit(EXIT_FAILURE);}
    for(int x = 0; x < row; x++){
        // for each row create memory block as container for columns
        arr[x] = malloc(column * type_size);
        if(arr[x] == NULL){perror("Failed to allocate for columns"); exit(EXIT_FAILURE);}

        for(int y = 0; y < column; y++){
            // initialize arr
            arr[x][y] = 0;
        }
    }
    return arr;
}

// returns palette index based on attribute lower 2 bits
uchar8_t get_palette(struct M6502* computer, uchar8_t attributes){
    uchar8_t bit0 = attributes & 1; // return bit 0
    uchar8_t bit1 = (attributes >> 1) & 1; // return bit 1
    uchar8_t palette_result = (bit1 << 1) + bit0; // add bit 1 and 0
    return palette_result; // palette 0-3
}

// returns 16 byte array from ppu address range based on input index
uchar8_t* get_pattern(struct M6502* computer, uchar8_t pattern_index){
    uchar8_t* pattern = malloc(16);
    if(pattern == NULL) exit(EXIT_FAILURE);
    for(int p = 0; p < 16; p++) // pattern indicies are offset by 16 bytes
        pattern[p] = PPU_address[p + (16 * pattern_index)];
    return pattern;
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
byte 1 | byte 9 = the shape in binary, then taking least 
significant bit or byte 1 and byte 9  say 10 = 2 so color index 2
*/

/*
Note for byte 1 of sprite oam
For 8x8 sprites, this is the tile number of this sprite within the pattern table selected in bit 3 of PPUCTRL ($2000)

Each 8x8 sprite is represented by 16 bytes in the pattern table. The first 8 bytes represent the low bits of the color
value for each pixel, and the next 8 bytes represent the high bits of the color value for each pixel

The third byte of the OAM data for a sprite specifies a palette number (from 0 to 3) 
in its lower two bits. This selects one of four palettes
*/

/*
my NES list to get graphics working

nametables:
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
    uchar8_t virtual_screen[240][256];
    uchar8_t palette_picker[240][256];

    int x, y, i;
    virtual_screen[y+i][x+i];// = sprite[i][i]
*/