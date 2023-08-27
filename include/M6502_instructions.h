#include "config.h"
#include "M6502.h"

void execute_instruction(struct M6502* computer, ushort16_t program_size);

void ADC(struct M6502* computer, uchar8_t mode);
void BRK();
void CLC();
void LDA(struct M6502* computer, uchar8_t mode);

// For instrution set:
// Case for all opcodes 
// Case points to a function
// Organize functions modularly possibly
// reference instruction functions for each specific case