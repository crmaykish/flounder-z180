#!/bin/sh

zcc -compiler sccz80 -v +z180 -startup=-1 -SO3 -clib=sdcc_iy -o monitor.bin \
    monitor.c \
    flounder.c \
    ps2.c

echo "Done!"