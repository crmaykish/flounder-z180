; Read and write the internal Z180 registers
in0 a, ($1F)
ld a, $80
out0 ($1F), a ; Set the Clock Divide register to XTAL/1 in the CPU Control Register
jp $0000
