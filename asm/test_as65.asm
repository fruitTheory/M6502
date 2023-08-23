    ; as65 Assemmbler Common Features:
    ; fast two-pass assembly
    ; supports binary, s-records or intel-hex output file formats
    ; optionally generates debug information file
    ; three segments (code, data, bss) for ROM-based programs
    ; 'struct' keyword for easy declarations
    ; instruction cycle count in listing
    ; full conditional assembly
    ; C-style expressions, accept base 2...36, using either %, $ and 0x prefixes or <base>#<value> format, 32-bit calculations
    ; full macro capability

    org $0200

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
