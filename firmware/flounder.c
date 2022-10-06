#include <stdlib.h>
#include <z180.h>
#include "flounder.h"

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

void flounder_init(void)
{
    // Set clock divide to XTAL/1 (i.e. run at the full oscillator frequency)
    z180_outp(CCR, 0x80);

    // Set 0 memory wait states and 1 I/O wait state
    z180_outp(DCNTL, 0);

    // Set CPU mode to full Z80 compatibility
    z180_outp(OMCR, 0);

    // Set ASCI 1 to use external clock source, 115200 bps at 1.8432 MHz
    z180_outp(CNTLB1, 0b00000111);

    // Set Transmit and Receive Enable ON for ASCI 1, 8-bit, no parity, 1 stop bit
    z180_outp(CNTLA1, 0b01100100);

    // Set PIO port B to output mode
    z180_outp(PORTB_CMD, 0b00001111);

    // Set PIO port B to zero
    z180_outp(PORTB_DATA, 0x00);
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

char *parse_param(char *s, char delim, size_t n)
{
    uint16_t i = 0;
    while (s[i] != delim && i < n)
    {
        i++;
    }

    if (i == (n - 1))
    {
        return 0;
    }

    return s + i + 1;
}
