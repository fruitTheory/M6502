#include "M6502_general.h"
#include <stdlib.h>

// converts input to 8 bit binary - allocates 9 bytes of memory 
char8_t* convert_to_binary(uchar8_t input_n){
    char8_t* binary = (char8_t*)malloc(9); // 8 bits + null terminator
    if(!binary) return NULL; // Check if memory created

    for (char8_t i = 7; i >= 0; i--)
        binary[7 - i] = ((input_n >> i) & 1) + '0';
    binary[8] = '\0'; // Add Null terminator

    return binary;
}