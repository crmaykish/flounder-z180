#!/bin/sh

rm -f *.bin *.map *.def monitor

zcc -v +z180 -startup=1 -SO3 -clib=sdcc_iy -g -m -o monitor -create-app \
    monitor.c \
    flounder.c \
    ps2.c \
    utils.asm

echo "Done!"
