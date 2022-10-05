CPLD0: equ $C0      ; First memory mapped CPLD register (I/O space)

PUBLIC _cpld_read

_cpld_read:
    in0 l, (CPLD0)
    ret
