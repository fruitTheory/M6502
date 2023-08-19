section .data
    hello db 'H', 'E', 'L', 'L', 'O'

section .bss
    destination resb 10 ; Reserve 10 bytes for the destination

section .text
    global main
    main:
        ; Load the address of the "HELLO" string into rsi
        mov rsi, hello
        ; Load the address of the destination into rdi
        mov rdi, destination
        ; Copy the "HELLO" string into the destination
        movsd
        movsb

        ; Return from main
        xor eax, eax ; Return value: 0
        ret
