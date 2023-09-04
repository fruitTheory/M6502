#include <stdio.h>
#include "config.h"
#include <stdbool.h>

int main(){

    unsigned char num1;
    unsigned char num2;
    unsigned short result;

    num1 = 0xFF;
    num2 = 0x05;

    printf("num1: %04X\n", num1);
    printf("num2: %04X\n", num2);

    result = num1 + num2;

    printf("num3: %04X\n", result);

    if(result > 0xFF) puts("something");

    // 0101 0101
    0b01010100 & flag_carry_bit ? puts("true"):puts("false");
    //printf("test: %04X\n", test);

}

