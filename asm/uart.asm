CNTLA0: equ $00     ; ASCI Control Register A Channel 0
CNTLA1: equ $01     ; ASCI Control Register A Channel 1
CNTLB0: equ $02     ; ASCI Control Register B Channel 0
CNTLB1: equ $03     ; ASCI Control Register B Channel 1
STAT0: equ $04      ; ASCI Status Channel 0
STAT1: equ $05      ; ASCI Status Channel 1
TDR0: equ $06       ; ASCI Transmit Data Register Channel 0
TDR1: equ $07       ; ASCI Transmit Data Register Channel 1
TSR0: equ $08       ; ASCI Receive Data Register Channel 0
TSR1: equ $09       ; ASCI Receive Data Register Channel 1
ASEXT1: equ $13     ; ASCI Extension Control Register Channel 1
CCR: equ $1F        ; CPU Control Register
DCNTL: equ $32      ; DMA/Wait Control Register
OMCR: equ $3E       ; Operation Mode Control Register

    org $0000

    ; Clear operating mode, I think this is more Z80 like
    ld a, $00
    out0 (OMCR), a

    ; Set Wait states to 0
    ld a, $00
    out0 (DCNTL), a

    ; Set ASCI to use external clock source
    ld a, 0b00000111
    out0 (CNTLB1), a

    ; Clear ASCI extension register, sets external clock scaling to 1/16
    ld a, $00
    out0 (ASEXT1), a

    ; Set Transmit and Receive Enable ON for ASCI 0, 8-bit, no parity, 1 stop bit
    ld a, 0b01100100
    out0 (CNTLA1), a

; TODO: system needs RAM before subroutines will work
; ram emulation might work at lower speed

newline:
    in0 a, (STAT1)
    bit 1, a  ; Check TDRE bit, if 1, send byte, else keep waiting
    jp z, newline
    ld a, $0A
    out0 (TDR1), a
return:
    in0 a, (STAT1)
    bit 1, a  ; Check TDRE bit, if 1, send byte, else keep waiting
    jp z, return
    ld a, $0D
    out0 (TDR1), a

    ld b, 'A'
loop:
    in0 a, (STAT1)
    bit 1, a  ; Check TDRE bit, if 1, send byte, else keep waiting
    jp z, loop

    ld a, b
    out0 (TDR1), a
    inc b
    ld a, b
    cp 'Z'+1
    jp z, newline

    jp loop
