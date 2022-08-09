CNTLA0: equ $00     ; ASCI Control Register A Channel 0
CNTLA1: equ $01     ; ASCI Control Register A Channel 1
CNTLB0: equ $02     ; ASCI Control Register B Channel 0
CNTLB1: equ $03     ; ASCI Control Register B Channel 1
STAT0: equ $04      ; ASCI Status Channel 0
STAT1: equ $05      ; ASCI Status Channel 1
TDR0: equ $06       ; ASCI Transmit Data Register Channel 0
TDR1: equ $07       ; ASCI Transmit Data Register Channel 1
RDR0: equ $08       ; ASCI Receive Register Channel 0
RDR1: equ $09       ; ASCI Receive Register Channel 1

PUBLIC _asci1_init, _asci1_putc, _asci1_getc

_asci1_init:
    ; Set ASCI 1 to use external clock source, 115200 bps at 1.8432 MHz
    ld a, %00000111
    out0 (CNTLB1), a

    ; Set Transmit and Receive Enable ON for ASCI 1, 8-bit, no parity, 1 stop bit
    ld a, %01100100
    out0 (CNTLA1), a

    ret

; Write a byte to ASCI1
_asci1_putc:
    in0 a, (STAT1)
    bit 1, a  ; Check TDRE bit, if 1, send byte, else keep waiting
    jp z, _asci1_putc

    ; Pop serial byte off the stack
    ld hl, 2
    add hl, sp
    ld a, (hl) ; Load the serial byte into A

    ; Write serial byte to ASCI
    out0 (TDR1), a

    ret

; Read a byte from ASCI1 (blocks until a byte is received)
_asci1_getc:
    in0 a, (STAT1)
    bit 7, a
    jp z, _asci1_getc

    in0 l, (RDR1)

    ret
