
    org $0100

    lda #$48    ; Load ASCII value for 'H'
    ;sta $06     ; Store at address $0000

    lda #$45    ; Load ASCII value for 'E'
    ;sta $01     ; Store at address $0001

    lda #$4C    ; Load ASCII value for 'L'
    ;sta $02     ; Store at address $0002

    lda #$4C    ; Load ASCII value for 'L'
    ;sta $03     ; Store at address $0003

    lda #$4F    ; Load ASCII value for 'O'
    ;sta $04     ; Store at address $0004

    sta $3000,x     ;Store accumulator

    jmp $1234

    brk         ; Break (end of program)
