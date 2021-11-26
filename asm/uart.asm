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
CCR: equ $1F        ; CPU Control Register

    ; Baudrate is 5760 with dafault clock settings

    di

    ld a, 0b00000000
    out0 (CNTLB1), a

    ; Set Transmit and Receive Enable ON for ASCI 0, 8-bit, no parity, 1 stop bit
    ld a, 0b01100100
    out0 (CNTLA1), a

UART_Reset:
    ld b, '!'

UART_Loop:
    in0 a, (STAT1)
    and 0b00000010  ; Check TDRE bit, if 1, send byte, else keep waiting
    jp z, UART_Loop

    out0 (TDR1), b

    jp UART_Loop
