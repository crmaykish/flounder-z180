#!/bin/sh

rm -f *.bin *.map *.def monitor

zcc -v +z180 -startup=1 -SO3 -clib=sdcc_iy -g -m -o monitor -create-app \
    monitor.c \
    flounder.c \
    ps2.c \
    interrupts.asm \
    -pragma-define:CRT_INTERRUPT_MODE=2 \
    -pragma-define:CRT_ENABLE_EIDI=0x1 \
    -pragma-define:CRT_ORG_VECTOR_TABLE=0xFB00 \
    -pragma-define:CRT_ORG_CODE=0 \
    -pragma-define:CRT_INCLUDE_PREAMBLE=0

echo "Done!"
