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

// Check if result is negative or positive, set flag accordingly, else clear flag
void check_flag_Carry(struct M6502* computer, short16_t test_against){
    // this is a little sketchy, check if positive first, then force check if negative
    // if neither positive nor negative it will clear the flag which should work both cases
    if(test_against > 0xFF)set_flag(computer, CARRY), puts("Carry Positive!");
    test_against < 0x00 ? set_flag(computer, CARRY),puts("Carry Negative!"):
    clear_flag(computer, CARRY),puts("Carry Cleared!");
}

// Check input against 0, if equal then set the Zero flag, else clear flag
void check_flag_Z(struct M6502* computer, uchar8_t test_against){

    test_against == 0 ? set_flag(computer, ZERO):clear_flag(computer, ZERO);
}
// Check if bit 7 of input is set, and then sets flag if true, else clear flag
void check_flag_N(struct M6502* computer, uchar8_t test_against){

    flag_negative_bit & test_against ? set_flag(computer, NEGATIVE):clear_flag(computer, NEGATIVE);
}

// check flag for zero and negative - shorthand function
void check_flag_ZN(struct M6502* computer, uchar8_t test_against){
    check_flag_Z(computer, test_against);
    check_flag_N(computer, test_against);
}

// check if a flag is set returns 1 or 0, can double as a 'is this bit set?' for cpu
// please use status register as default test case, otherwise specify another value
uchar8_t is_flag_set(uchar8_t FLAG, uchar8_t test_against){
    uchar8_t ret; 
    switch(FLAG)
    {
        case CARRY:
            // returns 1 if set or 0 if not
            ret = test_against & flag_carry_bit;
            return ret;
            break;
        case ZERO:
            ret = test_against & flag_zero_bit;
            return ret;
            break;
        case INTERRUPT:
            break;
        case DECIMAL:
            break;
        case BREAK:
            break;
        case OVERFLOW:
            ret = test_against & flag_overflow_bit;
            return ret;
            break;
        case NEGATIVE:
            // returns 1 if set or 0 if not
            ret = test_against & flag_negative_bit;
            return ret;
            break;
        default:
            puts("Error: not a valid flag");
            break;
    }

    return EXIT_FAILURE;
}

// set specific bit of input value - needs pointer to modify original value
uchar8_t set_bit(uchar8_t bit, uchar8_t* test_against){
        switch(bit)
    {
        case 0:
            *test_against |= flag_carry_bit;
            break;
        case 1:
            *test_against |= flag_zero_bit;
            break;
        case 2:
            *test_against |= flag_interrupt_bit;
            break;
        case 3:
            *test_against |= flag_decimal_bit;
            break;
        case 4:
            *test_against |= flag_break_bit;
            break;
        case 5:
            *test_against |= flag_ignored_bit;
            break;
        case 6:
            *test_against |= flag_overflow_bit;
            break;
        case 7:
            *test_against |= flag_negative_bit;
            break;
        default:
            puts("Error: use bit 0-7");
            break;
    }
    return EXIT_FAILURE;
}

// clear specific bit of input value - needs pointer to modify original value
uchar8_t clear_bit(uchar8_t bit, uchar8_t* test_against){
    switch(bit)
    {
        case 0:
            *test_against &= ~flag_carry_bit;
            break;
        case 1:
            *test_against &= ~flag_zero_bit;
            break;
        case 2:
            *test_against &= ~flag_interrupt_bit;
            break;
        case 3:
            *test_against &= ~flag_decimal_bit;
            break;
        case 4:
            *test_against &= ~flag_break_bit;
            break;
        case 5:
            *test_against &= ~flag_ignored_bit;
            break;
        case 6:
            *test_against &= ~flag_overflow_bit;
            break;
        case 7:
            *test_against &= ~flag_negative_bit;
            break;
        default:
            puts("Error: use bit 0-7");
            break;
    }
    return EXIT_FAILURE;
}

// checks if a bit is set, mainly used for PPU registers such as PPUCTRL, PPUMASK, PPUSTATUS, etc.
uchar8_t is_bit_set(uchar8_t bit, uchar8_t test_against){
    uchar8_t ret; 
    switch(bit)
    {
        case 0:
            // returns 1 if set or 0 if not
            ret = test_against & flag_carry_bit;
            return ret;
            break;
        case 1:
            ret = test_against & flag_zero_bit;
            return ret;
            break;
        case 2:
            ret = test_against & flag_interrupt_bit;
            return ret;
            break;
        case 3:
            ret = test_against & flag_decimal_bit;
            return ret;
            break;
        case 4:
            ret = test_against & flag_break_bit;
            return ret;
            break;
        case 5:
            ret = test_against & flag_ignored_bit;
            return ret;
            break;
        case 6:
            ret = test_against & flag_overflow_bit;
            return ret;
            break;
        case 7:
            ret = test_against & flag_negative_bit;
            return ret;
            break;
        default:
            puts("Error: use bit 0-7");
            break;
    }

    return EXIT_FAILURE;
}