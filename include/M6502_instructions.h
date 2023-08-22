#include "config.h"
#include "M6502.h"

void LDA_Immeadiate(struct M6502* M6502, uchar8_t user_value);
void LDX_Zero_Page(struct M6502* M6502, uchar8_t user_value);
void LDX_Zero_Page_X(struct M6502* M6502, uchar8_t user_value);
void LDA_Absolute(struct M6502* M6502, ushort16_t user_value);

void ADC(struct M6502* M6502, ushort16_t user_value, uchar8_t mode);
void BRK();

// For instrution set:
// Case for all opcodes 
// Case points to a function
// Organize functions modularly possibly
// reference instruction functions for each specific case