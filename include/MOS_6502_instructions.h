#include "config.h"
#include "MOS_6502.h"

void LDA_Immeadiate(struct MOS_6502* MOS_6502, uchar8_t user_value);
void LDX_Zero_Page(struct MOS_6502* MOS_6502, uchar8_t user_value);
void LDX_Zero_Page_X(struct MOS_6502* MOS_6502, uchar8_t user_value);
void LDA_Absolute(struct MOS_6502* MOS_6502, ushort16_t user_value);

// For instrution set:
// Case for all opcodes 
// Case points to a function
// Organize functions modularly possibly
// reference instruction functions for each specific case