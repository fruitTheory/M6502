.segment "HEADER"

  .byte $4E, $45, $53, $1A ; iNES header identifier
  .byte 2               ; 2x 16KB PRG code
  .byte 1               ; 1x  8KB CHR data
  .byte $01, $00        ; mapper 0, vertical mirroring

.segment "VECTORS"
  ;; When an NMI happens (once per frame if enabled) the label nmi:
  .addr nmi
  ;; When the processor first turns on or is reset, it will jump to the label reset:
  .addr reset
  ;; External interrupt IRQ (unused)
  .addr 0

; "nes" linker config requires a STARTUP section, even if it's empty
.segment "STARTUP"

; Main code segment for the program
.segment "CODE"

reset:
  sei		; disable IRQs 78
  cld		; disable decimal mode D8
  ldx #$40 ; A2
  stx $4017	; disable APU frame IRQ *E
  ldx #$ff 	; Set up stack A2
  txs		;  9A
  inx		; now X = 0 E8
  stx $2000	; disable NMI 8E
  stx $2001 	; disable rendering
  stx $4010 	; disable DMC IRQs

;; first wait for vblank to make sure PPU is ready
vblankwait1:
  bit $2002 ; 2C
  bpl vblankwait1 ; 10

clear_memory:
  lda #$00
  sta $0000, x
  sta $0100, x
  sta $0200, x
  sta $0300, x
  sta $0400, x
  sta $0500, x
  sta $0600, x
  sta $0700, x
  inx
  bne clear_memory

;; second wait for vblank, PPU is ready after this
vblankwait2:
  bit $2002
  bpl vblankwait2

main:
load_palettes:
  lda $2002 ;A9 -> 0
  lda #$3f
  sta $2006
  lda #$00
  sta $2006
  ldx #$00
@loop:
  lda palettes, x
  sta $2007
  inx
  cpx #$20
  bne @loop

enable_rendering:
  lda #%10000000	; Enable NMI $A9
  sta $2000       ; $8D
  lda #%00010000	; Enable Sprites $A9
  sta $2001       ; $8D

forever:
  jmp forever ; 4C leads to here gets called forever waiting for nmi

nmi:
  ldx #$00 	; Set SPR-RAM address to 0 $A2
  stx $2003 ; $8E - OAM r/w address
@loop:	
  lda hello, x 	; Load hello to SPR-RAM $A9
  sta $2004 ; $8D - OAM r/w data - so esstentially writing bytes to oam addresses
  inx       ; $E8
  cpx #$28  ; $E0
  bne @loop ; $F0
  rti       ; $40

hello:
  .byte $6c, $00, $00, $6c  ; this data represents sprite attributes
  .byte $6c, $01, $00, $76  ; OAM info on where sprites are positioned
  .byte $6c, $02, $00, $80  ; y position, index of tile, attribs 0-7b, x position
  .byte $6c, $02, $00, $8A
  .byte $6c, $03, $00, $94

  .byte $6c, $04, $00, $A3  ; W
  .byte $6c, $05, $00, $AC  ; o
  .byte $6c, $06, $00, $B6  ; r
  .byte $6c, $07, $00, $C0  ; l
  .byte $6c, $08, $00, $CA  ; d

palettes:
  ; Background Palette
  .byte $0f, $00, $00, $00  ; got background and sprite palette in ppu memory 3F00
  .byte $0f, $00, $00, $00
  .byte $0f, $00, $00, $00
  .byte $0f, $00, $00, $00

  ; Sprite Palette
  .byte $0f, $20, $00, $00
  .byte $0f, $00, $00, $00
  .byte $0f, $00, $00, $00
  .byte $0f, $00, $00, $00

; Character memory
.segment "CHARS"
  .byte %11000011	; $C3 - H (00)
  .byte %11000011 ; $C3
  .byte %11000011 ; $C3
  .byte %11111111 ; $FF
  .byte %11111111 ; $FF
  .byte %11000011 ; $C3
  .byte %11000011 ; $C3
  .byte %11000011 ; $C3
  .byte $00, $00, $00, $00, $00, $00, $00, $00

  .byte %11111111	; E (01)
  .byte %11111111
  .byte %11000000
  .byte %11111100
  .byte %11111100
  .byte %11000000
  .byte %11111111
  .byte %11111111
  .byte $00, $00, $00, $00, $00, $00, $00, $00

  .byte %11000000	; L (02)
  .byte %11000000
  .byte %11000000
  .byte %11000000
  .byte %11000000
  .byte %11000000
  .byte %11111111
  .byte %11111111
  .byte $00, $00, $00, $00, $00, $00, $00, $00

  .byte %01111110	; O (03)
  .byte %11100111
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte %11100111
  .byte %01111110
  .byte $00, $00, $00, $00, $00, $00, $00, $00

  ; W (04)
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte %11011011
  .byte %11011011
  .byte %11011011
  .byte %11111111
  .byte %11111111
  .byte $00, $00, $00, $00, $00, $00, $00, $00

  ; o (05)
  .byte %01111110
  .byte %11100111
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte %11100111
  .byte %01111110
  .byte $00, $00, $00, $00, $00, $00, $00, $00

  ; R (06)
  .byte %11111111
  .byte %11000011
  .byte %11000011
  .byte %11111111
  .byte %11000110
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte $00, $00, $00, $00, $00, $00, $00, $00

  ; L (07)
  .byte %11000000
  .byte %11000000
  .byte %11000000
  .byte %11000000
  .byte %11000000
  .byte %11000000
  .byte %11111111
  .byte %11111111
  .byte $00, $00, $00, $00, $00, $00, $00, $00

  ; D (08)
  .byte %11111100
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte %11000011
  .byte %11111100
  .byte $00, $00, $00, $00, $00, $00, $00, $00


