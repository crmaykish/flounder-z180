EXTERN _main

    org $0000

    ld sp, $FFFF

    ; Set clock divide to XTAL/1 (i.e. run at the full oscillator frequency)
    ld a, $80
    out0 ($1F), a

    call _main
