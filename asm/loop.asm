; Load a value into A, increment a few times, and jump back to 0x0000
ld a,$AA
inc a
inc a
inc a
jmp $00
