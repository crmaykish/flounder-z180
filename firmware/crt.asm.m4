EXTERN _main

    org $0000

    ld sp, $FFFF

    ld a, $80
    out0 ($1F), a

    call _main
