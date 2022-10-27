# Flounder Z180 Computer

![Flounder Z180 Computer](/media/flounder_rev1a.png)

Flounder is a standalone single-board computer built around the Zilog Z180 processor.

It features:
- Clock speed up to 33 MHz (overclockable to 40+)
- 512 KB of both ROM and SRAM
- Two RS-232 serial ports
- 16 GPIO pins
- PS/2 keyboard interface
- Support for 40x4 character LCD display
- CH376S USB storage

## Hardware
An Altera EPM7064 (or EPM7128) 84-pin CPLD provides glue logic and additional interfacing options. An expansion header breaks out address, data, and control lines to support additional hardware development.

## Software
All software development is done in C with the z88dk toolkit. Programs are flashed to and run from the EEPROM.

A hardware monitor program gives access to basic memory and I/O operations including: peek, poke, and memdump.

## Resources
1. [Hackaday Project](https://hackaday.io/project/187588-flounder-z180-sbc)
2. [Z8S180 Datasheet](http://www.zilog.com/docs/z180/z8s180ps.pdf)
3. [z88dk](https://github.com/z88dk/z88dk)
