    org $0000
    jp init

; Block 0x00 - 0xFF is reserved for other interrupt modes

; Block 0x100 - 0x1FF is reserved for interrupt vector map

ALIGN 0x104

    defw asm_isr_prt0       ; Put the address of the PRT0 timer ISR at the right vector

; Code definition starts at 0x200

ALIGN 0x200

init:
    di
    im 2

    ld sp, $FFFF            ; Set stack pointer to top of RAM

    ld a, $01
    ld I, a                 ; Set base interrupt vector to 0x100

    EXTERN _main
    call _main              ; Jump to C code main()

asm_isr_prt0:               ; PRT0 timer interrupt handler
    di                      ; disable interrupts
    exx                     ; swap to shadow registers
    EXTERN _ISR_prt0
    call _ISR_prt0          ; call the C code handler for this interrupt
    exx                     ; swap the shadow registers back
    ei                      ; enable interrupts again
    reti

PUBLIC _interrupts_enable, _interrupts_disable

_interrupts_enable:
    ei
    ret

_interrupts_disable:
    di
    ret
