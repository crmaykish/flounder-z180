SECTION code_user

PUBLIC _interrupts_enable
_interrupts_enable:
    ei
    ret

PUBLIC _interrupts_enable
_interrupts_disable:
    di
    ret

PUBLIC _z180_int_asci0
_z180_int_asci0:
    ; TODO: ASCI0 interrupt handler
    reti

PUBLIC _z180_int_asci1
_z180_int_asci1:
    ; TODO: ASCI1 interrupt handler
    reti

PUBLIC _z180_int_prt0
_z180_int_prt0:
    ; PRT0 interrupt handler
    di
    exx

    EXTERN _ISR_prt0
    call _ISR_prt0

    exx
    ei
    reti
