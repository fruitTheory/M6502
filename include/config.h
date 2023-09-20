#pragma once

typedef unsigned char uchar8_t;
typedef unsigned short ushort16_t;
typedef unsigned long ulong64_t;
typedef const char* cString;
typedef short short16_t;
typedef char char8_t;

#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1

#define cpu_max_address 65535 // M6502's 16-bit address bus can only handle this much
#define stack_max_size 256
#define ppu_max_address 16384

#define zero_page_start 0x0000
#define stack_start 0x0100 // decimal(byte) 256 - Start of stack
#define stack_end 0x01FF // decimal(byte) 511 - Use as offset for programs
#define program_initial_load 0x0200 // decimal(byte) 512 - Programs first byte
#define NES_initial_load 0x8000 // default load address for cartridges

#define accumulator (computer->cpu.registers.AC)
#define program_counter (computer->cpu.registers.PC)
#define CPU_address (computer->cpu.memory.address)
#define x_register (computer->cpu.registers.X)
#define y_register (computer->cpu.registers.Y)
#define status_register (computer->cpu.registers.SR)

#define stack_pointer (computer->cpu.registers.SP) // points to stack related memory address
#define stack_container (computer->cpu.stack.stack)

#define NMI 0xFFFA // Non-maskable interrupt handler
#define RESET 0xFFFC // Power-on/Reset
#define IRQ 0xFFFE // Interrupt Request Handler - BRK

#define increment_true 0 // used with get_memory_word function to determine if PC should be incremented
#define increment_false -1 // used with get_memory_word function to determine if PC should be incremented

// NES section open

#define PPU (computer->NES_ppu.ppu)
#define PPU_address (computer->ppu.memory.address)
#define PPU_ctrl (computer->ppu.registers.CTRL)
#define PPU_mask (computer->ppu.registers.MASK)
#define PPU_status (computer->ppu.registers.STATUS)
#define PPU_oam_addr (computer->ppu.registers.OAM_ADDR)
#define PPU_oam_data (computer->ppu.registers.OAM_DATA)
#define PPU_scroll (computer->ppu.registers.SCROLL)
#define PPU_addr (computer->ppu.registers.ADDR)
#define PPU_data (computer->ppu.registers.DATA)

// NES secton close

// enumeration to represent addressing modes
typedef enum{
    ACCUMULATOR,
    IMMEDIATE,
    ZERO_PAGE,
    ZERO_PAGE_X,
    ZERO_PAGE_Y,
    ABSOLUTE,
    ABSOLUTE_X,
    ABSOLUTE_Y,
    INDIRECT,
    INDIRECT_X,
    INDIRECT_Y,
    RELATIVE
}address_mode;
/* Notes: 
- Indirects are more like a redirect or pointer to another location
- Immeadiate is only literal value regardless of hex or decimal
- X Y are just offsets based on X Y register
- Zero page 8 bit Absolute 16 bit
- Accumulator acts directly upon Accumulator
- Relative is just a 8 bit relative offset if branch is true
*/

#define flag_negative_bit 0x80 // check bit 7 - 1000 0000
#define flag_overflow_bit 0x40 // check bit 6 - 0100 0000
#define flag_ignored_bit 0x20 // check bit 5 - 0010 0000
#define flag_break_bit 0x10 // check bit 4 - 0001 0000
#define flag_decimal_bit 0x08 // check bit 3 - 0000 1000
#define flag_interrupt_bit 0x04 // check bit 2 - 0000 0100
#define flag_zero_bit 0x02 // check bit 1 - 0000 0010
#define flag_carry_bit 0x01 // check bit 0 - 0000 0001

// 8 bits of processor status flags stored in status register
typedef enum {
    CARRY, // Set if the last operation carried (addition) or borrowed (subtraction)
    ZERO, //  Set if the result of the operation is zero, otherwise clear
    INTERRUPT, // If set, disables all maskable interrupts
    DECIMAL, // Used to enable Binary Coded Decimal (BCD) mode in the processor, ignored in 8-bit M6502s
    BREAK, // Only appears when a BRK instruction has been executed and an interrupt has been generated
    IGNORED, // Ignored
    OVERFLOW, //  Set if the signed result overflows, otherwise clear
    NEGATIVE // Set if the result is negative, clear if positive
}SR_Flags;

// Instructions 

#define ADC_IMMEDIATE_D 0x69 // Add with Carry - Immediate
#define ADC_ZERO_PAGE_D 0x65 // Add with Carry - Zero Page
#define ADC_ZERO_PAGE_X_D 0x75 // Add with Carry - Zero Page, X
#define ADC_ABSOLUTE_D 0x6D // Add with Carry - Absolute
#define ADC_ABSOLUTE_X_D 0x7D // Add with Carry - Absolute, X
#define ADC_ABSOLUTE_Y_D 0x79 // Add with Carry - Absolute, Y
#define ADC_INDIRECT_X_D 0x61 // Add with Carry - (Indirect, X)
#define ADC_INDIRECT_Y_D 0x71 // Add with Carry - (Indirect), Y

#define AND_IMMEDIATE_D 0x29 // Logical AND - Immediate
#define AND_ZERO_PAGE_D 0x25 // Logical AND - Zero Page
#define AND_ZERO_PAGE_X_D 0x35 // Logical AND - Zero Page, X
#define AND_ABSOLUTE_D 0x2D // Logical AND - Absolute
#define AND_ABSOLUTE_X_D 0x3D // Logical AND - Absolute, X
#define AND_ABSOLUTE_Y_D 0x39 // Logical AND - Absolute, Y
#define AND_INDIRECT_X_D 0x21 // Logical AND - (Indirect, X)
#define AND_INDIRECT_Y_D 0x31 // Logical AND - (Indirect), Y

#define ASL_ACCUMULATOR_D 0x0A // Arithmetic Shift Left - Accumulator
#define ASL_ZERO_PAGE_D 0x06 // Arithmetic Shift Left - Zero Page
#define ASL_ZERO_PAGE_X_D 0x16 // Arithmetic Shift Left - Zero Page, X
#define ASL_ABSOLUTE_D 0x0E // Arithmetic Shift Left - Absolute
#define ASL_ABSOLUTE_X_D 0x1E // Arithmetic Shift Left - Absolute, X

#define BCC_RELATIVE_D 0x90 // Branch if Carry Clear - Relative
#define BCS_RELATIVE_D 0xB0 // Branch if Carry Set - Relative
#define BEQ_RELATIVE_D 0xF0 // Branch if Equal - Relative
#define BMI_RELATIVE_D 0x30 // Branch if Minus - Relative
#define BNE_RELATIVE_D 0xD0 // Branch if Not Equal - Relative
#define BPL_RELATIVE_D 0x10 // Branch if Plus - Relative
#define BVC_RELATIVE_D 0x50 // Branch if Overflow Clear - Relative
#define BVS_RELATIVE_D 0x70 // Branch if Overflow Set - Relative

#define BIT_ZERO_PAGE_D 0x24 // Bit Test - Zero Page
#define BIT_ABSOLUTE_D 0x2C // Bit Test - Absolute

#define BRK_D 0x00 // Force an Interrupt - Implied

#define CLC_D 0x18 // Clear Carry Flag - Implied
#define CLD_D 0xD8 // Clear Decimal Mode - Implied
#define CLI_D 0x58 // Clear Interrupt Disable - Implied
#define CLV_D 0xB8 // Clear Overflow Flag - Implied

#define CMP_IMMEDIATE_D 0xC9 // Compare Accumulator - Immediate
#define CMP_ZERO_PAGE_D 0xC5 // Compare Accumulator - Zero Page
#define CMP_ZERO_PAGE_X_D 0xD5 // Compare Accumulator - Zero Page, X
#define CMP_ABSOLUTE_D 0xCD // Compare Accumulator - Absolute
#define CMP_ABSOLUTE_X_D 0xDD // Compare Accumulator - Absolute, X
#define CMP_ABSOLUTE_Y_D 0xD9 // Compare Accumulator - Absolute, Y
#define CMP_INDIRECT_X_D 0xC1 // Compare Accumulator - (Indirect, X)
#define CMP_INDIRECT_Y_D 0xD1 // Compare Accumulator - (Indirect, Y)

#define CPX_IMMEDIATE_D 0xE0 // Compare X Register - Immediate
#define CPX_ZERO_PAGE_D 0xE4 // Compare X Register - Zero Page
#define CPX_ABSOLUTE_D 0xEC // Compare X Register - Absolute

#define CPY_IMMEDIATE_D 0xC0 // Compare Y Register - Immediate
#define CPY_ZERO_PAGE_D 0xC4 // Compare Y Register - Zero Page
#define CPY_ABSOLUTE_D 0xCC // Compare Y Register - Absolute

#define DEC_ZERO_PAGE_D 0xC6 // Decrement a Memory Location - Zero Page
#define DEC_ZERO_PAGE_X_D 0xD6 // Decrement a Memory Location - Zero Page, X
#define DEC_ABSOLUTE_D 0xCE // Decrement a Memory Location - Absolute
#define DEC_ABSOLUTE_X_D 0xDE // Decrement a Memory Location - Absolute, X
#define DEX_D 0xCA // Decrement the X Register - Implied
#define DEY_D 0x88 // Decrement the Y Register - Implied

#define EOR_IMMEDIATE_D 0x49 // Exclusive OR - Immediate
#define EOR_ZERO_PAGE_D 0x45 // Exclusive OR - Zero Page
#define EOR_ZERO_PAGE_X_D 0x55 // Exclusive OR - Zero Page, X
#define EOR_ABSOLUTE_D 0x4D // Exclusive OR - Absolute
#define EOR_ABSOLUTE_X_D 0x5D // Exclusive OR - Absolute, X
#define EOR_ABSOLUTE_Y_D 0x59 // Exclusive OR - Absolute, Y
#define EOR_INDIRECT_X_D 0x41 // Exclusive OR - (Indirect, X)
#define EOR_INDIRECT_Y_D 0x51 // Exclusive OR - (Indirect, Y)

#define INC_ZERO_PAGE_D 0xE6 // Increment a Memory Location - Zero Page
#define INC_ZERO_PAGE_X_D 0xF6 // Increment a Memory Location - Zero Page, X
#define INC_ABSOLUTE_D 0xEE // Increment a Memory Location - Absolute
#define INC_ABSOLUTE_X_D 0xFE // Increment a Memory Location - Absolute, X

#define INX_D 0xE8 // Increment the X Register - Implied
#define INY_D 0xC8 // Increment the Y Register - Implied

#define JMP_ABSOLUTE_D 0x4C // Jump to Another Location - Absolute
#define JMP_INDIRECT_D 0x6C // Jump to Another Location - Indirect
#define JSR_ABSOLUTE_D 0x20 // Jump to a Subroutine - Absolute

#define LDA_IMMEDIATE_D 0xA9 // Load Accumulator - Immediate
#define LDA_ZERO_PAGE_D 0xA5 // Load Accumulator - Zero Page
#define LDA_ZERO_PAGE_X_D 0xB5 // Load Accumulator - Zero Page, X
#define LDA_ABSOLUTE_D 0xAD // Load Accumulator - Absolute
#define LDA_ABSOLUTE_X_D 0xBD // Load Accumulator - Absolute, X
#define LDA_ABSOLUTE_Y_D 0xB9 // Load Accumulator - Absolute, Y
#define LDA_INDIRECT_X_D 0xA1 // Load Accumulator - (Indirect, X)
#define LDA_INDIRECT_Y_D 0xB1 // Load Accumulator - (Indirect), Y

#define LDX_IMMEDIATE_D 0xA2 // Load X Register - Immediate
#define LDX_ZERO_PAGE_D 0xA6 // Load X Register - Zero Page
#define LDX_ZERO_PAGE_Y_D 0xB6 // Load X Register - Zero Page, Y
#define LDX_ABSOLUTE_D 0xAE // Load X Register - Absolute
#define LDX_ABSOLUTE_Y_D 0xBE // Load X Register - Absolute, Y

#define LDY_IMMEDIATE_D 0xA0 // Load Y Register - Immediate
#define LDY_ZERO_PAGE_D 0xA4 // Load Y Register - Zero Page
#define LDY_ZERO_PAGE_X_D 0xB4 // Load Y Register - Zero Page, X
#define LDY_ABSOLUTE_D 0xAC // Load Y Register - Absolute
#define LDY_ABSOLUTE_X_D 0xBC // Load Y Register - Absolute, X

#define LSR_ACCUMULATOR_D 0x4A // Logical Shift Right - Accumulator
#define LSR_ZERO_PAGE_D 0x46 // Logical Shift Right - Zero Page
#define LSR_ZERO_PAGE_X_D 0x56 // Logical Shift Right - Zero Page, X
#define LSR_ABSOLUTE_D 0x4E // Logical Shift Right - Absolute
#define LSR_ABSOLUTE_X_D 0x5E // Logical Shift Right - Absolute, X

#define NOP_D 0xEA // No Operation - Implied

#define ORA_IMMEDIATE_D 0x09 // Logical Inclusive OR - Immediate
#define ORA_ZERO_PAGE_D 0x05 // Logical Inclusive OR - Zero Page
#define ORA_ZERO_PAGE_X_D 0x15 // Logical Inclusive OR - Zero Page, X
#define ORA_ABSOLUTE_D 0x0D // Logical Inclusive OR - Absolute
#define ORA_ABSOLUTE_X_D 0x1D // Logical Inclusive OR - Absolute, X
#define ORA_ABSOLUTE_Y_D 0x19 // Logical Inclusive OR - Absolute, Y
#define ORA_INDIRECT_X_D 0x01 // Logical Inclusive OR - (Indirect, X)
#define ORA_INDIRECT_Y_D 0x11 // Logical Inclusive OR - (Indirect, X)

#define PHA_D 0x48 // Push Accumulator on Stack - Implied
#define PHP_D 0x08 // Push Processor Status on Stack - Implied
#define PLA_D 0x68 // Pull Accumulator from Stack - Implied
#define PLP_D 0x28 // Pull Processor Status from Stack - Implied

#define ROL_ACCUMULATOR_D 0x2A // Rotate Left - Accumulator
#define ROL_ZERO_PAGE_D 0x26 // Rotate Left - Zero Page
#define ROL_ZERO_PAGE_X_D 0x36 // Rotate Left - Zero Page, X
#define ROL_ABSOLUTE_D 0x2E // Rotate Left - Absolute
#define ROL_ABSOLUTE_X_D 0x3E // Rotate Left - Absolute, X

#define ROR_ACCUMULATOR_D 0x6A // Rotate Right - Accumulator
#define ROR_ZERO_PAGE_D 0x66 // Rotate Right - Zero Page
#define ROR_ZERO_PAGE_X_D 0x76 // Rotate Right - Zero Page, X
#define ROR_ABSOLUTE_D 0x6E // Rotate Right - Absolute
#define ROR_ABSOLUTE_X_D 0x7E // Rotate Right - Absolute, X

#define RTI_D 0x40 // Return from Interrupt - Implied
#define RTS_D 0x60 // Return from Subroutine - Implied

#define SBC_IMMEDIATE_D 0xE9 // Subtract with Carry - Immediate
#define SBC_ZERO_PAGE_D 0xE5 // Subtract with Carry - Zero Page
#define SBC_ZERO_PAGE_X_D 0xF5 // Subtract with Carry - Zero Page, X
#define SBC_ABSOLUTE_D 0xED // Subtract with Carry - Absolute
#define SBC_ABSOLUTE_X_D 0xFD // Subtract with Carry - Absolute, X
#define SBC_ABSOLUTE_Y_D 0xF9 // Subtract with Carry - Absolute, Y
#define SBC_INDIRECT_X_D 0xE1 // Subtract with Carry - (Indirect, X)
#define SBC_INDIRECT_Y_D 0xF1 // Subtract with Carry - (Indirect), Y

#define SEC_D 0x38 // Set Carry Flag - Implied
#define SED_D 0xF8 // Set Decimal Mode Flag - Implied
#define SEI_D 0x78 // Set Interrupt Disable Flag - Implied

#define STA_ZERO_PAGE_D 0x85 // Store Accumulator - Zero Page
#define STA_ZERO_PAGE_X_D 0x95 // Store Accumulator - Zero Page, X
#define STA_ABSOLUTE_D 0x8D // Store Accumulator - Absolute
#define STA_ABSOLUTE_X_D 0x9D // Store Accumulator - Absolute, X
#define STA_ABSOLUTE_Y_D 0x99 // Store Accumulator - Absolute, Y
#define STA_INDIRECT_X_D 0x81 // Store Accumulator - (Indirect, X)

#define STX_ZERO_PAGE_D 0x86 // Store X Register - Zero Page
#define STX_ZERO_PAGE_Y_D 0x96 // Store X Register - Zero Page, Y
#define STX_ABSOLUTE_D 0x8E // Store X Register - Absolute

#define STY_ZERO_PAGE_D 0x84 // Store Y Register - Zero Page
#define STY_ZERO_PAGE_X_D 0x94 // Store Y Register - Zero Page, X
#define STY_ABSOLUTE_D 0x8C // Store Y Register - Absolute

#define TAX_D 0xAA // Transfer Accumulator to X - Implied
#define TAY_D 0xA8 // Transfer Accumulator to Y - Implied
#define TSX_D 0xBA // Transfer Stack Pointer to X - Implied
#define TXA_D 0x8A // Transfer X to Accumulator - Implied
#define TXS_D 0x9A // Transfer X to Stack Pointer - Implied
#define TYA_D 0x98 // Transfer Y to Accumulator - Implied
