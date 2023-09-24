    .org $8000       ; set program start at memory location $8000

start:
    ; Load/Store instructions
    lda #$12        ; $A9: load accumulator
    ldx #$34        ; $A2: load X register
    ldy #$56        ; $A0: load Y register
    sta $0200       ; $8D: store accumulator
    stx $0201       ; $8E: store X register
    sty $0202       ; $8C: store Y register

    ; Arithmetic/Logic operations
    adc #$10        ; $69: add with carry
    sbc #$08        ; $E9: subtract with carry
    cmp #$20        ; $C9: compare accumulator
    cpx #$15        ; $E0: compare X register
    cpy #$25        ; $C0: compare Y register
    and #$AA        ; $29: bitwise AND
    ora #$55        ; $09: bitwise OR
    eor #$5A        ; $49: bitwise XOR
    bit $0200       ; $2C: bit test

    ; Shift/Rotate operations
    asl             ; $0A: arithmetic shift left accumulator
    lsr             ; $4A: logical shift right accumulator
    rol             ; $2A: rotate left accumulator
    ror             ; $6A: rotate right accumulator

    ; Increment/Decrement operations
    inc $0203       ; $EE: increment memory
    inx             ; $E8: increment X register
    iny             ; $C8: increment Y register
    dec $0204       ; $CE: decrement memory
    dex             ; $CA: decrement X register
    dey             ; $88: decrement Y register

    ; Branch operations
    bcc branch      ; $90: branch on carry clear
    bcs branch      ; $B0: branch on carry set
    beq branch      ; $F0: branch if equal
    bmi branch      ; $30: branch if minus
    bne branch      ; $D0: branch if not equal
    bpl branch      ; $10: branch if plus
    bvc branch      ; $50: branch if overflow clear
    bvs branch      ; $70: branch if overflow set

branch:
    nop             ; $EA: no operation

    ; Transfer instructions
    tax             ; $AA: transfer accumulator to X
    tay             ; $A8: transfer accumulator to Y
    txa             ; $8A: transfer X to accumulator
    tya             ; $98: transfer Y to accumulator

    ; Stack operations
    tsx             ; $BA: transfer stack pointer to X
    txs             ; $9A: transfer X to stack pointer
    pha             ; $48: push accumulator on stack
    php             ; $08: push processor status on stack
    pla             ; $68: pull accumulator from stack
    plp             ; $28: pull processor status from stack

    ; Flags/Control operations
    clc             ; $18: clear carry flag
    cld             ; $D8: clear decimal flag
    cli             ; $58: clear interrupt disable flag
    clv             ; $B8: clear overflow flag
    sec             ; $38: set carry flag
    sed             ; $F8: set decimal flag
    sei             ; $78: set interrupt disable flag

    jsr myRoutine  ; $20 (followed by 2-byte address of myRoutine)
    nop            ; $EA
    sei            ; $78
    ; ... other operations...
    cli            ; $58
    jmp continue

myRoutine:
    lda #$05       ; $A9 05
    sta $0200      ; $8D 00 02
    rts            ; $60

continue:    
        ; Immediate addressing
    lda #$0A       ; $A9 0A
    adc #$05       ; $69 05

    ; Zero Page addressing
    sta $00        ; $85 00
    ldy $01        ; $A4 01

    ; Zero Page,X addressing
    ldx #$02       ; $A2 02
    stx $03        ; $86 03
    lda $00,x      ; $B5 00

    ; Absolute addressing
    jmp exampleLabel ; $4C (followed by 2-byte address of exampleLabel)

    ; Absolute,X and Absolute,Y addressing
    ldx #$05       ; $A2 05
    ldy #$03       ; $A0 03
    sta $0300,x    ; $9D 00 03
    lda $0300,y    ; $B9 00 03

    ; Indirect addressing (for JMP)
    jmp ($FFFA)    ; $6C FA FF

    ; Indexed Indirect (using X)
    lda ($02, x)   ; $A1 02

    ; Indirect Indexed (using Y)
    ldx #$00       ; $A2 00
    lda ($00),y    ; $B1 00

exampleLabel:
    sec            ; $38
    sed            ; $F8
    clc            ; $18
    cld            ; $D8

    ; Increment/Decrement
    inc $0200      ; $EE 00 02
    dec $0200      ; $CE 00 02

    ; Shifts and Rotates
    asl            ; $0A
    lsr $0200      ; $4E 00 02
    rol            ; $2A
    ror $0200      ; $6E 00 02

    ; brk     ;00
    
    ; rti

    lda #$88    ;A9 

    ora ($55, x) ;$01 

    ;brk

