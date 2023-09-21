#include "ppu_general.h"
#include "M6502_flags.h"
#include <stdlib.h>
#include <stdio.h>

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