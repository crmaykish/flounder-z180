EXTERN _main

    org $0000

    ld sp, $0FFF

    ld a, $80
    out0 ($1F), a

    call _main
