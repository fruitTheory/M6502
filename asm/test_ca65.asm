    .org $0200

    lda #$48  ; Load ASCII value for 'H'

    lda #$45    ; Load ASCII value for 'E'

decrement_loop:
    sta $0200    ; Store the accumulator value to memory location $0200 (arbitrary location for this example)
    dex          ; Decrement X register
    bne skip     ; If X is not zero, skip the next instruction
    dec $0200    ; Decrement value at memory location $0200

skip:
    dey          ; Decrement Y register
    beq end      ; If Y is zero, end the loop

    sec          ; Set the carry flag
    sbc #$01     ; Subtract 1 from the accumulator
    bpl decrement_loop ; If result is positive, loop again

end:

    lda $10        ; Load the accumulator with the value at memory location $10
    jsr ADD_VALUES ; Call the subroutine to add another value to the accumulator
    sta $12        ; Store the result in memory location $12
    brk         ; Break (end of program)
    
ADD_VALUES:
    adc $11        ; Add the value at memory location $11 to the accumulator
    rts            ; Return from subroutine

