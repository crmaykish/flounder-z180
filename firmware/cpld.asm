CPLD_PREFIX: equ $40
CPLD0: equ $00      ; First memory mapped CPLD register (I/O space)

PUBLIC _cpld_read

_cpld_read:
    ld b, CPLD_PREFIX
    ld c, CPLD0
    in a, (c)
    ld l, a
    ret
