    .org $0200

    lda #$48  


    lda #$45    ; Load ASCII value for 'E'
    ;sta $01     ; Store at address $0001

    lda #$4C    ; Load ASCII value for 'L'
    ;sta $02     ; Store at address $0002

    lda #$4C    ; Load ASCII value for 'L'
    ;sta $03     ; Store at address $0003

    lda #$4F    ; Load ASCII value for 'O'
    ;sta $04     ; Store at address $0004

    lda $0045   ; zerpage addy

    lda $0262   ; absolute addy

    lda $50,X ; zeropage offset by X

    lda $0272,X   ; absolute offset by X

    lda ($40, X)    ; load a byte indirectly from memory

    lda ($40),Y     ;Load a byte indirectly from memory

    sta $3000,X     ;Store accumulator

    jmp $1234

    clc

    brk         ; Break (end of program)