EXTERN _main

    org $0000

    ld sp, $FFFF

    ; Set clock divide to XTAL/1 (i.e. run at the full oscillator frequency)
    ; CCR register
    ld a, $80
    out0 ($1F), a

    ; Set 0 memory wait states and 1 I/O wait state
    ; DCNTL register
    ld a, $0
    out0 ($32), a

    call _main
