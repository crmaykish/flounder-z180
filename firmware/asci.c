#include "asci.h"
#include <stdlib.h>
#include <z180.h>
#include "flounder.h"

void asci0_putc(char a)
{
    while ((z180_inp(STAT0) & 0b00000010) == 0)
    {
    }

    z180_outp(TDR0, a);
}

char asci0_getc()
{
    while ((z180_inp(STAT0) & 0b10000000) == 0)
    {
    }

    return z180_inp(RDR0);
}

void asci1_putc(char a)
{
    while ((z180_inp(STAT1) & 0b00000010) == 0)
    {
    }

    z180_outp(TDR1, a);
}

char asci1_getc()
{
    while ((z180_inp(STAT1) & 0b10000000) == 0)
    {
    }

    return z180_inp(RDR1);
}

void uart_print(char *s)
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

void uart_print_dec(uint32_t n)
{
    char buffer[11] = {0};

    utoa((unsigned int)n, buffer, 10);
    uart_print(buffer);
}

void uart_print_hex(uint32_t n)
{
    char buffer[11] = {0};

    if (n < 0x10)
    {
        asci1_putc('0');
    }

    utoa((unsigned int)n, buffer, 16);
    uart_print(buffer);
}

uint16_t uart_readline(char *buffer, bool echo)
{
    char in = 0;
    uint16_t i = 0;

    while (in != 0x0A && in != 0x0D)
    {
        in = asci1_getc();

        if (echo)
        {
            asci1_putc(in);
        }

        buffer[i] = in;
        i++;
    }

    buffer[i] = 0;

    return i;
}
