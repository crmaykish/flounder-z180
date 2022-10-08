#!/bin/sh

rm -f *.bin monitor

zcc -compiler sccz80 -v +z180 -startup=1 -SO3 -clib=sdcc_iy -o monitor -create-app \
    monitor.c \
    flounder.c \
    ps2.c

# Why is this artifact just an empty file?
rm monitor

echo "Done!"
