#include <stdlib.h>
#include "flounder.h"
#include "asci.h"

static char decbuf[11] = {0};

void init_flounder()
{
    asci1_init();
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
    utoa((unsigned int)n, decbuf, 16);
    print(decbuf);
}

void newline()
{
    asci1_putc(0x0A);
    asci1_putc(0x0D);
}