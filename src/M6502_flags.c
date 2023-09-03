#include "M6502_flags.h"
#include "M6502.h"
#include <stdio.h>


// sets provided status register flag
void set_flag(struct M6502* computer, uchar8_t FLAG){

    // Another way could be to shift 1 << 6 - 0000 0001 -> 0100 0000
    switch(FLAG)
    {
        case CARRY:
            status_register |= flag_carry_bit;
            break;
        case ZERO:
            status_register |= flag_zero_bit;
            break;
        case INTERRUPT:
            status_register |= flag_interrupt_bit;
            break;
        case DECIMAL:
            status_register |= flag_decimal_bit;
            break;
        case BREAK:
            status_register |= flag_break_bit;
            break;
        case IGNORED:
            break;
        case OVERFLOW:
            status_register |= flag_overflow_bit;
            break;
        case NEGATIVE:
            status_register |= flag_negative_bit; // OR operation is inclusive
            break;
        default:
            puts("Error: not a valid flag");
        break;
    }
}

void set_flags_all(struct M6502* computer){
    set_flag(computer, CARRY);
    set_flag(computer, ZERO);
    set_flag(computer, INTERRUPT);
    set_flag(computer, DECIMAL);
    set_flag(computer, BREAK);
    set_flag(computer, OVERFLOW);
    set_flag(computer, NEGATIVE);
}

// clears provided status register flag
void clear_flag(struct M6502* computer, uchar8_t FLAG){
    // xor also works here as it returns 1 if only one of the bits is 1
    // 0010 0110 = 0010 0110 ^ 0000 0100 = 0010 0010
    // 0000 0100 -> 1111 1011 & 0010 0110 = 0010 0010
    // invert the set bit and & against SR, all remain same 
    // except the one that is compared against 0 aka inverted bit
    switch(FLAG)
    {
        case CARRY:
            status_register &= ~flag_carry_bit;
            break;
        case ZERO:
            status_register &= ~flag_zero_bit;
            break;
        case INTERRUPT:
            status_register &= ~flag_interrupt_bit;
            break;
        case DECIMAL:
            status_register &= ~flag_decimal_bit;
            break;
        case BREAK:
            status_register &= ~flag_break_bit;
            break;
        case IGNORED:
            break;
        case OVERFLOW:
            status_register &= ~flag_overflow_bit;
            break;
        case NEGATIVE:
            status_register &= ~flag_negative_bit;
            break;
        default:
            puts("Error: not a valid flag");
        break;
    }
}

// check if provided flag needs to be set, this also sets that flag
void check_flag(struct M6502* computer, uchar8_t FLAG, uchar8_t test_against){

    switch(FLAG)
    {
        case CARRY:
            break;
        case ZERO:
            //printf("SR: %04X\n", status_register);
            if(test_against == 0){
                //puts("Set Zero flag!");
                set_flag(computer, ZERO);
            }else{
                //puts("Cleared Zero flag!");
                clear_flag(computer, ZERO);
            }
            //printf("SR: %04X\n", status_register);
            break;
        case INTERRUPT:
            break;
        case DECIMAL:
            break;
        case BREAK:
            break;
        case IGNORED:
            break;
        case OVERFLOW:
            break;
        case NEGATIVE:
            //printf("SR: %04X\n", status_register);
            if(flag_negative_bit & test_against){
                //puts("Sign is negative!");
                set_flag(computer, NEGATIVE);
            }else{
                //puts("Cleared Negative sign!");
                clear_flag(computer, NEGATIVE);
            }
            //printf("SR: %04X\n", status_register);
            break;
        default:
            puts("Error: not a valid flag");
        break;
    }
}

// check flag for zero and negative - shorthand function
void check_flag_ZN(struct M6502* computer, uchar8_t test_against){
    check_flag(computer, ZERO, test_against);
    check_flag(computer, NEGATIVE, test_against);
}