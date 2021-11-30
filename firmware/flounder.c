#include <stdlib.h>
#include "flounder.h"
#include "asci.h"

static char decbuf[11] = {0};

void init_flounder()
{
    asci1_init();
}

uint8_t peek(uint16_t addr)
{
    return *(uint8_t *)addr;
}

void poke(uint16_t addr, uint8_t val)
{
    *(uint8_t *)(addr) = val;
}

void putc(char c)
{
    asci1_putc(c);
}

void print(char *s)
{
    char i = 0;
    char c = s[i];

    while (c != 0)
    {
        asci1_putc(c);
        i++;
        c = s[i];
    }
}

void print_dec(uint32_t n)
{
    utoa((unsigned int)n, decbuf, 10);
    print(decbuf);
}

void print_hex(uint32_t n)
{
    if (n < 0x10)
    {
        putc('0');
    }
    
    utoa((unsigned int)n, decbuf, 16);
    print(decbuf);
}

void newline()
{
    asci1_putc(0x0A);
    asci1_putc(0x0D);
}

char getc()
{
    return asci1_getc();
}

uint16_t readline(char *buffer, bool echo)
{
    char in = 0;
    uint16_t i = 0;

    while (in != 0x0A && in != 0x0D)
    {
        in = getc();

        if (echo)
        {
            putc(in);
        }

        buffer[i] = in;
        i++;
    }

    buffer[i] = 0;

    return i;
}
