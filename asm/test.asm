MATCH1:  .byte "?"         ; Matches exactly 1 character
MATCHN: .byte "*"         ; Matches any string (including "")
PATTERN: .word $2000       ; Address of pattern
STR:   .byte $6          ; Pointer to string to match

PATTERNMATCH:
        LDX #$00        ; X is an index in the pattern
        LDY #$FF        ; Y is an index in the string
NEXT:    LDA PATTERN,X   ; Look at next pattern character
        CMP #MATCHN     ; Is it a star?
        BEQ STAR        ; Yes, do the complicated stuff
        INY             ; No, let's look at the string
        CMP #MATCH1     ; Is the pattern caracter a ques?
        BNE REG         ; No, it's a regular character
        LDA (STR),Y     ; Yes, so it will match anything
        BEQ FAIL        ;  except the end of string
REG:     CMP (STR),Y     ; Are both characters the same?
        BNE FAIL        ; No, so no match
        INX             ; Yes, keep checking
        CMP #0          ; Are we at end of string?
        BNE NEXT        ; Not yet, loop
FOUND:   RTS             ; Success, return with C=1

STAR:    INX             ; Skip star in pattern
        CMP PATTERN,X   ; String of stars equals one star
        BEQ STAR        ;  so skip them also
STLOOP:  TXA             ; We first try to match with * = ""
        PHA             ;  and grow it by 1 character every
        TYA             ;  time we loop
        PHA             ; Save X and Y on stack
        JSR NEXT        ; Recursive call
        PLA             ; Restore X and Y
        TAY
        PLA
        TAX
        BCS FOUND       ; We found a match, return with C=1
        INY             ; No match yet, try to grow * string
        LDA (STR),Y     ; Are we at the end of string?
        BNE STLOOP      ; Not yet, add a character
FAIL:    CLC             ; Yes, no match found, return with C=0
        RTS