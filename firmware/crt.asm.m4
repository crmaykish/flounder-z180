; =============================
; CPU Control Registers
; =============================

CCR: equ $1F
DCNTL: equ $32
OMCR: equ $3E

; =============================
; Reset handler
; =============================

    org $0000           ; Code starts at address 0
    ld sp, $FFFF        ; Set stack pointer to top of RAM

    ld a, $80
    out0 (CCR), a       ; Set clock divide to XTAL/1 (i.e. run at the full oscillator frequency)

    ld a, $0
    out0 (DCNTL), a     ; Set 0 memory wait states and 1 I/O wait state
    
    ld a, %00000000
    out0 (OMCR), a      ; Set CPU mode to full Z80 compatibility

EXTERN _main
    call _main          ; Jump to C code main()
