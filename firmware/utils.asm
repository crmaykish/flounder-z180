PUBLIC _interrupts_enable, _interrupts_disable, _get_i, _set_i, _asm_isr_prt0

SECTION CODE

_interrupts_enable:
    ei
    ret

_interrupts_disable:
    di
    ret

_get_i:
    ld a, I
    ld l, a
    ret

_set_i:
    ld I, a
    ret

_asm_isr_prt0:
    di
    exx

    EXTERN _ISR_prt0
    call _ISR_prt0

    exx
    ei
    reti

; send A to PIO port B
pio:
    ld b, $20
    ld c, $01
    out (c), a
    ret
