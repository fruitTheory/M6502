#pragma once

#include "config.h"

typedef struct 
{
    ushort16_t PC;  // program counter - 16 bit - points to the next instruction to be executed
    uchar8_t AC;   // accumulator - 8 bit - used for all arithmetic and logical operations (with the exception of increments and decrements)
    uchar8_t X;    // X register - 8 bit - most commonly used to hold counters or offsets for accessing memory
    uchar8_t Y;    // Y register - 8 bit - available for holding counter or offsets memory access and supports the same set of memory load, save and compare operations as wells as increments and decrements
    uchar8_t SR;   // status register - 8 bit - stores status flags
    uchar8_t SP;   // stack pointer - 8 bit - holds the low 8 bits of the next free location on the stack

}M6502_registers;

// 8 bits of processor status flags stored in status register
enum SR_Flags{
    Carry, // Set if the last operation carried (addition) or borrowed (subtraction)
    Zero, //  Set if the result of the operation is zero, otherwise clear
    Interrupt, // If set, disables all maskable interrupts
    Decimal, // Used to enable Binary Coded Decimal (BCD) mode in the processor, ignored in 8-bit M6502s
    Break, // Only appears when a BRK instruction has been executed and an interrupt has been generated
    Ignored, // Ignored
    Overflow, //  Set if the signed result overflows, otherwise clear
    Negative // Set if the result is negative, clear if positive
};


// The program counter is incremented when an instruction is read for execution
// the program counter needs to be incremented by amount of data needed by instruction


// check bit 7 - 1000 0000
#define flag_negative_bit 0x80
// check bit 6 - 0100 0000
#define flag_overflow_bit 0x40
// check bit 5 - 0010 0000
#define flag_ignored_bit 0x20
// check bit 4 - 0001 0000
#define flag_break_bit 0x10 
// check bit 3 - 0000 1000
#define flag_decimal_bit 0x08 
// check bit 2 - 0000 0100
#define flag_interrupt_bit 0x04 
// check bit 1 - 0000 0010
#define flag_zero_bit 0x02 
// check bit 0 - 0000 0001
#define flag_carry_bit 0x01


/*
if(flag & flag_carry_bit)
0x80 // check bit 7
0x40 // check bit 6
0x20 // check bit 5
0x10 // check bit 4
0x08 // check bit 3
0x04 // check bit 2
0x02 // check bit 1
0x01 // check bit 0
*/

/*
The zero flag (Z) indicates a value of all zero bits and the negative flag (N) indicates the presence of a set sign bit
in bit-position 7. These flags are always updated, whenever a value is transferred to a CPU register (A,X,Y)
and as a result of any logical ALU operations. The Z and N flags are also updated by increment and
decrement operations acting on a memory location.

The carry flag (C) flag is used as a buffer and as a borrow in arithmetic operations. Any comparisons will update this
additionally to the Z and N flags, as do shift and rotate operations.

All arithmetic operations update the Z, N, C and V flags.

The overflow flag (V) indicates overflow with signed binary arithmetics. As a signed byte represents a range of -128 to +127,
an overflow can never occur when the operands are of opposite sign, since the result will never exceed this range.
Thus, overflow may only occur, if both operands are of the same sign. Then, the result must be also of the same sign.
Otherwise, overflow is detected and the overflow flag is set. (I.e., both operands have a zero in the sign position at
bit 7, but bit 7 of the result is 1, or, both operands have the sign-bit set, but the result is positive.)

The decimal flag (D) sets the ALU to binary coded decimal (BCD) mode for additions and subtractions (ADC, SBC).

The interrupt inhibit flag (I) blocks any maskable interrupt requests (IRQ).

The break flag (B) is not an actual flag implemented in a register, and rather appears only, when the status register is pushed
onto or pulled from the stack. When pushed, it will be 1 when transfered by a BRK or PHP instruction,
and zero otherwise (i.e., when pushed by a hardware interrupt). When pulled into the status register (by PLP or on RTI),
it will be ignored.
In other words, the break flag will be inserted, whenever the status register is transferred to the stack by software (BRK or PHP),
and will be zero, when transferred by hardware. Since there is no actual slot for the break flag, it will be always ignored,
when retrieved (PLP or RTI). The break flag is not accessed by the CPU at anytime and there is no internal representation.
Its purpose is more for patching, to discern an interrupt caused by a BRK instruction from a normal interrupt initiated by hardware.

Any of these flags (but the break flag) may be set or cleared by dedicated instructions. Moreover, there are branch instructions
to conditionally divert the control flow depending on the respective state of the Z, N, C or V flag.

*/