.org $8000

;.segment "HEADER"
  ; .byte "NES", $1A      ; iNES header identifier
  ; .byte $4E, $45, $53, $1A

    lda $0200       ; AD Load a value from memory address $0200 into the accumulator
    bit $0200       ; 2C Test bits against accumulator; sets the N (negative) flag if bit 7 of $0200 is set

    bpl is_positive ; 10 Branch if the N flag is clear (i.e., value is positive)

negative_code:
    lda #$FF        ; A9 Load the value 0xFF into the accumulator
    sta $0300       ; 8D Store accumulator value in memory address $0300
    jmp end         ; 4C Jump to the end

is_positive:
    lda #$01        ; Load the value 0x01 into the accumulator
    sta $0300       ; Store accumulator value in memory address $0300

end:
    lda $8000       ; AD
    ; Program continues or halts here
