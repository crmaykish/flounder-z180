#!/bin/sh

echo "Assembling ${1}..."
z80asm -mz180 -b "${1}"
echo "Done!"

exit 0