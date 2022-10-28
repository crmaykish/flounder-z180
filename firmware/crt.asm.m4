; This CRT only applies to RAM applications, not applications running from ROM

EXTERN _main
    org $B000
    call _main
