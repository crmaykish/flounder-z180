PORTA_DATA: equ $80
PORTB_DATA: equ $81
PORTA_CMD: equ $82
PORTB_CMD: equ $83

PUBLIC _pio_init, _pio_setb

_pio_init:
    ld a, %00001111
    out0 (PORTB_CMD), a  ; Set port B to output mode
    ret

_pio_setb:
    ld hl, 2
    add hl, sp
    ld a, (hl)
    out0 (PORTB_DATA), a
    ret
