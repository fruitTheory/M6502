    .org $0200

    lda #$48  ; Load ASCII value for 'H'

    lda #$45    ; Load ASCII value for 'E'

    lda $0045   ; zerpage addy

    lda $0262   ; absolute addy

    lda $50,X ; zeropage offset by X

    lda $0272,X   ; absolute offset by X

    lda ($40, X)    ; load a byte indirectly from memory

    lda ($40),Y     ;Load a byte indirectly from memory

    sta $3000,X     ;Store accumulator

    ;jmp $1234   ; jump to location

    clc         ; clear carry flag

    ;pha         ; push accum to stack
    ;php         ; push status register to stack

    lda $10        ; Load the accumulator with the value at memory location $10
    jsr ADD_VALUES ; Call the subroutine to add another value to the accumulator
    sta $12        ; Store the result in memory location $12
    brk         ; Break (end of program)
    
ADD_VALUES:
    adc $11        ; Add the value at memory location $11 to the accumulator
    rts            ; Return from subroutine

