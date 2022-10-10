SECTION code_user

PUBLIC _interrupts_disable
_interrupts_disable:
    di
    im 0
    ret

PUBLIC _interrupts_enable
_interrupts_enable:
    push af
    ld a, $FB
    ld I, a
    im 2
    ei

    pop af
    ret
