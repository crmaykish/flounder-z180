    org $0000           ; Code starts at address 0
    ld sp, $FFFF        ; Set stack pointer to top of RAM

EXTERN _main
    call _main          ; Jump to C code main()
