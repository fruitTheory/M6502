#include "config.h"
#include "M6502.h"
#include "M6502_instructions.h"
#include <memory.h>
#include <stdio.h>

/*
    General M6502 functions
*/

// initialize the processor
void M6502_init(struct M6502* computer){
    // set everything in memory to 0
    memset(computer, 0, sizeof(&computer));
    M6502_stack_init(computer);
    status_register = 0b01010101; // temporary SR init 0x55 - 0101 0101
}

extern inline void PC_increment(struct M6502* computer);
extern inline void PC_decrement(struct M6502* computer);

// returns an opcode
uchar8_t instruction_fetch(struct M6502* computer){
    uchar8_t opcode = memory_address[program_counter];
    
    printf("PC: %04X\n", program_counter);
    extern ushort16_t global_pc;
    global_pc = program_counter;

    printf("opcode: %02X\n", opcode);
    extern uchar8_t global_opcode;
    global_opcode = opcode;

    return opcode;
}

// executes all program instructions
void execute_instructions(struct M6502* computer, ushort16_t program_size){
    ushort16_t initial_program_counter = program_counter;
    // the loop continually subtracts the new PC from old until the difference reaches the program size-1
    if((program_counter - initial_program_counter < program_size)){
        printf("prog size: %i (PS-IB): %i byteCount: %i\n", program_size, program_counter - initial_program_counter, instruction_byte_count);
        uchar8_t opcode = instruction_fetch(computer);
        analyze_opcode(computer, opcode);
        PC_increment(computer);
        // Note for this function - it goes to analyze_opcode and PC is +1 for instruction call
        // If the instruction needs to return byte, it will already be at that address
        // If the instruction needs to return word, the PC is +1 from M6502_get_word()
        // When it returns from instruction the PC is +1 to prepare for next opcode
        // Implied and Accumulator will need to decrement PC by -1 as they are only 1 byte in total
    }
}

// check if page was crossed when the input address offsets to a new address
// register_n means the offset amount but typically register x or y is used as the offset
// n is the amount of cycles added if page crossed
void check_page(struct M6502* computer, ushort16_t input_address, uchar8_t register_n, uchar8_t n){
    
    ushort16_t input_address_offset = input_address + register_n;

    float old_page = input_address/256; // this naturally truncates 
    float new_page = input_address_offset/256;
    (old_page != new_page) ? cycle_push(n) : cycle_push(0); // if new page != old page +n cycle
    //(old_page != new_page) ? puts("cyc: 1") :puts("cyc: 0"); // if new page != old page +1 cycle
}

// determines which opcode was returned
void analyze_opcode(struct M6502* computer, uchar8_t opcode){

    // Initial byte to get off the instruction to the user input
    PC_increment(computer);

    switch (opcode)
    {
        // ADC - Add Memory to Accumulator with Carry
        case ADC_IMMEDIATE_D: 
            ADC(computer, IMMEDIATE);
            break;
        case ADC_ZERO_PAGE_D: 
            ADC(computer, ZERO_PAGE);
            break;
        case ADC_ZERO_PAGE_X_D: 
            ADC(computer, ZERO_PAGE_X);
            break;
        case ADC_ABSOLUTE_D: 
            ADC(computer, ABSOLUTE);
            break;
        case ADC_ABSOLUTE_X_D: 
            ADC(computer, ABSOLUTE_X);
            break;
        case ADC_ABSOLUTE_Y_D: 
            ADC(computer, ABSOLUTE_Y);
            break;
        case ADC_INDIRECT_X_D: 
            ADC(computer, INDIRECT_X);
            break;
        case ADC_INDIRECT_Y_D: 
            ADC(computer, INDIRECT_Y);
            break;

        // AND - Logical AND
        case AND_IMMEDIATE_D: 
            AND(computer, IMMEDIATE);
            break;
        case AND_ZERO_PAGE_D: 
            AND(computer, ZERO_PAGE);
            break;
        case AND_ZERO_PAGE_X_D: 
            AND(computer, ZERO_PAGE_X);
            break;
        case AND_ABSOLUTE_D: 
            AND(computer, ABSOLUTE);
            break;
        case AND_ABSOLUTE_X_D: 
            AND(computer, ABSOLUTE_X);
            break;
        case AND_ABSOLUTE_Y_D: 
            AND(computer, ABSOLUTE_Y);
            break;
        case AND_INDIRECT_X_D: 
            AND(computer, INDIRECT_X);
            break;
        case AND_INDIRECT_Y_D: 
            AND(computer, INDIRECT_Y);
            break;

        // ASL - Arithmetic Shift Left
        case ASL_ACCUMULATOR_D: 
            PC_decrement(computer);
            ASL(computer, ACCUMULATOR);
            break;
        case ASL_ZERO_PAGE_D: 
            ASL(computer, ZERO_PAGE);
            break;
        case ASL_ZERO_PAGE_X_D: 
            ASL(computer, ZERO_PAGE_X);
            break;
        case ASL_ABSOLUTE_D: 
            ASL(computer, ABSOLUTE);
            break;
        case ASL_ABSOLUTE_X_D: 
            ASL(computer, ABSOLUTE_X);
            break;

        // BCC - Branch if Carry Clear
        case BCC_RELATIVE_D: 
            BCC(computer);
            break;
        // BCS - Branch if Carry Set
        case BCS_RELATIVE_D: 
            BCS(computer);
            break;
        // BEQ - Branch if Equal
        case BEQ_RELATIVE_D: 
            BEQ(computer);
            break;
            
        // BIT - Bit Test
        case BIT_ZERO_PAGE_D: 
            BIT(computer, ZERO_PAGE);
            break;
        case BIT_ABSOLUTE_D: 
            BIT(computer, ABSOLUTE);
            break;

        // BMI - Branch if Minus
        case BMI_RELATIVE_D: 
            BMI(computer);
            break;
        // BNE - Branch if Not Equal
        case BNE_RELATIVE_D: 
            BNE(computer);
            break;
        // BPL - Branch if Plus
        case BPL_RELATIVE_D: 
            BPL(computer);
            break;

        // BRK - Force Interrupt
        case BRK_D:
            PC_decrement(computer); 
            BRK(computer);
            break;

        // BVC - Branch if Overflow Clear
        case BVC_RELATIVE_D: 
            BVC(computer);
            break;
        // BVS - Branch if Overflow Set
        case BVS_RELATIVE_D: 
            BVS(computer);
            break;

        // CLC - Clear Carry Flag
        case CLC_D:
            PC_decrement(computer);
            CLC(computer);
            break;
        // CLD - Clear Decimal Mode
        case CLD_D: 
            PC_decrement(computer);
            CLD(computer);
            break;
        // CLI - Clear Interrupt Disable
        case CLI_D: 
            PC_decrement(computer);
            CLI(computer);
            break;
        // CLV - Clear Overflow Flag
        case CLV_D: 
            PC_decrement(computer);
            CLV(computer);
            break;

        // CMP - Compare Accumulator
        case CMP_IMMEDIATE_D: 
            CMP(computer, IMMEDIATE);
            break;
        case CMP_ZERO_PAGE_D: 
            CMP(computer, ZERO_PAGE);
            break;
        case CMP_ZERO_PAGE_X_D: 
            CMP(computer, ZERO_PAGE_X);
            break;
        case CMP_ABSOLUTE_D: 
            CMP(computer, ABSOLUTE);
            break;
        case CMP_ABSOLUTE_X_D: 
            CMP(computer, ABSOLUTE_X);
            break;
        case CMP_ABSOLUTE_Y_D: 
            CMP(computer, ABSOLUTE_Y);
            break;
        case CMP_INDIRECT_X_D: 
            CMP(computer, INDIRECT_X);
            break;
        case CMP_INDIRECT_Y_D: 
            CMP(computer, INDIRECT_Y);
            break;

        // CPX - Compare X Register
        case CPX_IMMEDIATE_D: 
            CPX(computer, IMMEDIATE);
            break;
        case CPX_ZERO_PAGE_D: 
            CPX(computer, ZERO_PAGE);
            break;
        case CPX_ABSOLUTE_D: 
            CPX(computer, ABSOLUTE);
            break;

        // CPY - Compare Y Register
        case CPY_IMMEDIATE_D: 
            CPY(computer, IMMEDIATE);
            break;
        case CPY_ZERO_PAGE_D: 
            CPY(computer, ZERO_PAGE);
            break;
        case CPY_ABSOLUTE_D: 
            CPY(computer, ABSOLUTE);
            break;

        // DEC - Decrement a Memory Location
        case DEC_ZERO_PAGE_D: 
            DEC(computer, ZERO_PAGE);
            break;
        case DEC_ZERO_PAGE_X_D: 
            DEC(computer, ZERO_PAGE_X);
            break;
        case DEC_ABSOLUTE_D: 
            DEC(computer, ABSOLUTE);
            break;
        case DEC_ABSOLUTE_X_D: 
            DEC(computer, ABSOLUTE_X);
            break;

        // DEX - Decrement the X Register
        case DEX_D: 
            PC_decrement(computer);
            DEX(computer);
            break;

        // DEY - Decrement the Y Register
        case DEY_D: 
            PC_decrement(computer);
            DEY(computer);
            break;

        // EOR - Exclusive OR
        case EOR_IMMEDIATE_D: 
            EOR(computer, IMMEDIATE);
            break;
        case EOR_ZERO_PAGE_D: 
            EOR(computer, ZERO_PAGE);
            break;
        case EOR_ZERO_PAGE_X_D: 
            EOR(computer, ZERO_PAGE_X);
            break;
        case EOR_ABSOLUTE_D: 
            EOR(computer, ABSOLUTE);
            break;
        case EOR_ABSOLUTE_X_D: 
            EOR(computer, ABSOLUTE_X);
            break;
        case EOR_ABSOLUTE_Y_D: 
            EOR(computer, ABSOLUTE_Y);
            break;
        case EOR_INDIRECT_X_D: 
            EOR(computer, INDIRECT_X);
            break;
        case EOR_INDIRECT_Y_D: 
            EOR(computer, INDIRECT_Y);
            break;

        // INC - Increment a Memory Location
        case INC_ZERO_PAGE_D: 
            INC(computer, ZERO_PAGE);
            break;
        case INC_ZERO_PAGE_X_D: 
            INC(computer, ZERO_PAGE_X);
            break;
        case INC_ABSOLUTE_D: 
            INC(computer, ABSOLUTE);
            break;
        case INC_ABSOLUTE_X_D: 
            INC(computer, ABSOLUTE_X);
            break;

        // INX - Increment the X Register
        case INX_D: 
            PC_decrement(computer);
            INX(computer);
            break;

        // INY - Increment the Y Register
        case INY_D: 
            PC_decrement(computer);
            INY(computer);
            break;

        // JMP - Jump to Another Location
        case JMP_ABSOLUTE_D: 
            JMP(computer, ABSOLUTE);
            break;
        case JMP_INDIRECT_D: 
            JMP(computer, INDIRECT);
            break;

        // JSR - Jump to a Subroutine
        case JSR_ABSOLUTE_D: 
            JSR(computer, ABSOLUTE);
            break;

        // LDA - Load Accumulator
        case LDA_IMMEDIATE_D: 
            LDA(computer, IMMEDIATE);
            break;
        case LDA_ZERO_PAGE_D: 
            LDA(computer, ZERO_PAGE);
            break;
        case LDA_ZERO_PAGE_X_D: 
            LDA(computer, ZERO_PAGE_X);
            break;
        case LDA_ABSOLUTE_D: 
            LDA(computer, ABSOLUTE);
            break;
        case LDA_ABSOLUTE_X_D: 
            LDA(computer, ABSOLUTE_X);
            break;
        case LDA_ABSOLUTE_Y_D: 
            LDA(computer, ABSOLUTE_Y);
            break;
        case LDA_INDIRECT_X_D: 
            LDA(computer, INDIRECT_X);
            break;
        case LDA_INDIRECT_Y_D: 
            LDA(computer, INDIRECT_Y);
            break;

        // LDX - Load X Register
        case LDX_IMMEDIATE_D: 
            LDX(computer, IMMEDIATE);
            break;
        case LDX_ZERO_PAGE_D: 
            LDX(computer, ZERO_PAGE);
            break;
        case LDX_ZERO_PAGE_Y_D: 
            LDX(computer, ZERO_PAGE_Y);
            break;
        case LDX_ABSOLUTE_D: 
            LDX(computer, ABSOLUTE);
            break;
        case LDX_ABSOLUTE_Y_D: 
            LDX(computer, ABSOLUTE_Y);
            break;

        // LDY - Load Y Register
        case LDY_IMMEDIATE_D: 
            LDY(computer, IMMEDIATE);
            break;
        case LDY_ZERO_PAGE_D: 
            LDY(computer, ZERO_PAGE);
            break;
        case LDY_ZERO_PAGE_X_D: 
            LDY(computer, ZERO_PAGE_X);
            break;
        case LDY_ABSOLUTE_D: 
            LDY(computer, ABSOLUTE);
            break;
        case LDY_ABSOLUTE_X_D: 
            LDY(computer, ABSOLUTE_X);
            break;

        // LSR - Logical Shift Right
        case LSR_ACCUMULATOR_D: 
            PC_decrement(computer);
            LSR(computer, ACCUMULATOR);
            break;
        case LSR_ZERO_PAGE_D: 
            LSR(computer, ZERO_PAGE);
            break;
        case LSR_ZERO_PAGE_X_D: 
            LSR(computer, ZERO_PAGE_X);
            break;
        case LSR_ABSOLUTE_D: 
            LSR(computer, ABSOLUTE);
            break;
        case LSR_ABSOLUTE_X_D: 
            LSR(computer, ABSOLUTE_X);
            break;

        // NOP - No Operation
        case NOP_D: 
            PC_decrement(computer);
            NOP(computer);
            break;

        // ORA - Logical Inclusive OR
        case ORA_IMMEDIATE_D: 
            ORA(computer, IMMEDIATE);
            break;
        case ORA_ZERO_PAGE_D: 
            ORA(computer, ZERO_PAGE);
            break;
        case ORA_ZERO_PAGE_X_D: 
            ORA(computer, ZERO_PAGE_X);
            break;
        case ORA_ABSOLUTE_D: 
            ORA(computer, ABSOLUTE);
            break;
        case ORA_ABSOLUTE_X_D: 
            ORA(computer, ABSOLUTE_X);
            break;
        case ORA_ABSOLUTE_Y_D: 
            ORA(computer, ABSOLUTE_Y);
            break;
        case ORA_INDIRECT_X_D: 
            ORA(computer, INDIRECT_X);
            break;
        case ORA_INDIRECT_Y_D: 
            ORA(computer, INDIRECT_Y);
            break;

        // PHA - Push Accumulator on Stack
        case PHA_D: 
            PC_decrement(computer);
            PHA(computer);
            break;

        // PHP - Push Processor Status on Stack
        case PHP_D: 
            PC_decrement(computer);
            PHP(computer);
            break;

        // PLA - Pull Accumulator from Stack
        case PLA_D: 
            PC_decrement(computer);
            PLA(computer);
            break;

        // PLP - Pull Processor Status from Stack
        case PLP_D: 
            PC_decrement(computer);
            PLP(computer);
            break;

        // ROL - Rotate Left
        case ROL_ACCUMULATOR_D: 
            PC_decrement(computer);
            ROL(computer, ACCUMULATOR);
            break;
        case ROL_ZERO_PAGE_D: 
            ROL(computer, ZERO_PAGE);
            break;
        case ROL_ZERO_PAGE_X_D: 
            ROL(computer, ZERO_PAGE_X);
            break;
        case ROL_ABSOLUTE_D: 
            ROL(computer, ABSOLUTE);
            break;
        case ROL_ABSOLUTE_X_D: 
            ROL(computer, ABSOLUTE_X);
            break;

        // ROR - Rotate Right
        case ROR_ACCUMULATOR_D: 
            PC_decrement(computer);
            ROR(computer, ACCUMULATOR);
            break;
        case ROR_ZERO_PAGE_D: 
            ROR(computer, ZERO_PAGE);
            break;
        case ROR_ZERO_PAGE_X_D: 
            ROR(computer, ZERO_PAGE_X);
            break;
        case ROR_ABSOLUTE_D: 
            ROR(computer, ABSOLUTE);
            break;
        case ROR_ABSOLUTE_X_D: 
            ROR(computer, ABSOLUTE_X);
            break;

        // RTI - Return from Interrupt
        case RTI_D: 
            PC_decrement(computer);
            RTI(computer);
            break;

        // RTS - Return from Subroutine
        case RTS_D: 
            PC_decrement(computer);
            RTS(computer);
            break;

        // SBC - Subtract Memory from Accumulator with Borrow
        case SBC_IMMEDIATE_D: 
            SBC(computer, IMMEDIATE);
            break;
        case SBC_ZERO_PAGE_D: 
            SBC(computer, ZERO_PAGE);
            break;
        case SBC_ZERO_PAGE_X_D: 
            SBC(computer, ZERO_PAGE_X);
            break;
        case SBC_ABSOLUTE_D: 
            SBC(computer, ABSOLUTE);
            break;
        case SBC_ABSOLUTE_X_D: 
            SBC(computer, ABSOLUTE_X);
            break;
        case SBC_ABSOLUTE_Y_D: 
            SBC(computer, ABSOLUTE_Y);
            break;
        case SBC_INDIRECT_X_D: 
            SBC(computer, INDIRECT_X);
            break;
        case SBC_INDIRECT_Y_D: 
            SBC(computer, INDIRECT_Y);
            break;

        // SEC - Set Carry Flag
        case SEC_D: 
            PC_decrement(computer);
            SEC(computer);
            break;

        // SED - Set Decimal Mode
        case SED_D: 
            PC_decrement(computer);
            SED(computer);
            break;

        // SEI - Set Interrupt Disable Status
        case SEI_D: 
            PC_decrement(computer);
            SEI(computer);
            break;

        // STA - Store Accumulator in Memory
        case STA_ZERO_PAGE_D: 
            STA(computer, ZERO_PAGE);
            break;
        case STA_ZERO_PAGE_X_D: 
            STA(computer, ZERO_PAGE_X);
            break;
        case STA_ABSOLUTE_D: 
            STA(computer, ABSOLUTE);
            break;
        case STA_ABSOLUTE_X_D: 
            STA(computer, ABSOLUTE_X);
            break;
        case STA_ABSOLUTE_Y_D: 
            STA(computer, ABSOLUTE_Y);
            break;
        case STA_INDIRECT_X_D: 
            STA(computer, INDIRECT_X);
            break;

        // STX - Store X Register in Memory
        case STX_ZERO_PAGE_D: 
            STX(computer, ZERO_PAGE);
            break;
        case STX_ZERO_PAGE_Y_D: 
            STX(computer, ZERO_PAGE_Y);
            break;
        case STX_ABSOLUTE_D: 
            STX(computer, ABSOLUTE);
            break;

        // STY - Store Y Register in Memory
        case STY_ZERO_PAGE_D: 
            STY(computer, ZERO_PAGE);
            break;
        case STY_ZERO_PAGE_X_D: 
            STY(computer, ZERO_PAGE_X);
            break;
        case STY_ABSOLUTE_D: 
            STY(computer, ABSOLUTE);
            break;

        // TAX - Transfer Accumulator to X
        case TAX_D: 
            PC_decrement(computer);
            TAX(computer);
            break;

        // TAY - Transfer Accumulator to Y
        case TAY_D: 
            PC_decrement(computer);
            TAY(computer);
            break;

        // TSX - Transfer Stack Pointer to X
        case TSX_D: 
            PC_decrement(computer);
            TSX(computer);
            break;

        // TXA - Transfer X to Accumulator
        case TXA_D: 
            PC_decrement(computer);
            TXA(computer);
            break;

        // TXS - Transfer X to Stack Pointer
        case TXS_D: 
            PC_decrement(computer);
            TXS(computer);
            break;

        // TYA - Transfer Y to Accumulator
        case TYA_D: 
            PC_decrement(computer);
            TYA(computer);
            break;

        // Not a legal opcode
        default:
            PC_decrement(computer);
            puts("Error: case not found");
            break;
    }
}



