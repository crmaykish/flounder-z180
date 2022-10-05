#!/bin/sh

if [ -z ${1} ]; then
    echo "Requires a program name. i.e. build.sh <program>"
    exit 1
fi

echo "Building ${1}.bin..."

zcc +z180 -startup=-1 -vn -SO3 -clib=new -o ${1}.bin asci.asm flounder.c ps2.c pio.asm cpld.asm ${1}.c

echo "Done!"