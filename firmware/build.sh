#!/bin/sh

zcc +z180 -startup=-1 -vn -SO3 -clib=new -o monitor.bin \
    monitor.c \
    hardware.asm \
    flounder.c \
    ps2.c

echo "Done!"