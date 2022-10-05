PIO_PREFIX: equ $20
PORTA_DATA: equ $00
PORTB_DATA: equ $01
PORTA_CMD: equ $02
PORTB_CMD: equ $03

PUBLIC _pio_init, _pio_setb

_pio_init:
    ld b, PIO_PREFIX ; load PIO prefix into B
    ld a, %00001111
    ld c, PORTB_CMD
    out (c), a  ; Set port B to output mode
    ret

_pio_setb:
    ld hl, 2
    add hl, sp
    ld a, (hl)
    
    ld b, PIO_PREFIX

    ld c, PORTB_DATA
    
    out (c), a
    
    ret
