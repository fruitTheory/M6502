#include <stdio.h>
#include "config.h"
#include <stdbool.h>

int main(){

    unsigned char num1;
    unsigned char num2;
    short result;

    num1 = 0x05;
    num2 = 0xFF;

    printf("num1: %04X\n", num1);
    printf("num2: %04X\n", num2);

    result = num1 - num2;

    printf("num3: %04X\n", result);
    printf("num3: %i\n", result);

    if(result < 0) puts("true");

    // 0101 0101
    //0b01010100 & flag_carry_bit ? puts("true"):puts("false");


}

