; Store a value in RAM, load it, manipulate it, store it, repeat
ld hl, $0400    ; load 0x400 into HL
ld a, $10       ; load 0x10 into A
ld (hl), a
loop:
    ld a, (hl)  ; load A from 0x400
    add a, $5   ; add 5 to A
    ld (hl), a  ; store A at 0x400

jp loop         ; jump to "loop"
