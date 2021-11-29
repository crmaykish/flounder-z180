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

    ; Set stack pointer to top of RAM (4095)
    ld sp, $0FFF

    ; Set PHI to EXTAL/1 (run the system at the rate of the clock source)
    ld a, $80
    out0 (CCR), a

    ; Set ASCI 1 to use external clock source
    ld a, %00000111
    out0 (CNTLB1), a

    ; Set Transmit and Receive Enable ON for ASCI 1, 8-bit, no parity, 1 stop bit
    ld a, %01100100
    out0 (CNTLA1), a

restart:
    ld b, $0A
    call asci1_putc
    ld b, $0D
    call asci1_putc
    ld b, 'A'
loop:
    call asci1_putc
    inc b
    ld a, b
    cp 'Z'+1
    jp z, restart
    jp loop

; Send B to ASCI 1
asci1_putc:
    in0 a, (STAT1)
    bit 1, a  ; Check TDRE bit, if 1, send byte, else keep waiting
    jp z, asci1_putc

    ld a, b
    out0 (TDR1), a
    ret
