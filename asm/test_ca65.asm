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

    brk         ; Break (end of program)