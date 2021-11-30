#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "asci.h"

#define SYSTEM_NAME "| === Flounder Z180 === |"

char decbuf[11] = {0};

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

bool is_prime(uint32_t p)
{
    uint32_t i = 2;

    if (p < 2)
    {
        return false;
    }
    else if (p == 2)
    {
        return true;
    }
    else
    {
        while (i < (p / 2) + 1)
        {
            if (p % i == 0)
            {
                return false;
            }
            i++;
        }
    }

    return true;
}

void newline()
{
    asci1_putc(0x0A);
    asci1_putc(0x0D);
}

int main()
{
    uint16_t current = 2;

    asci1_init();

    newline();
    print(SYSTEM_NAME);
    newline();

    while (1)
    {
        for (uint16_t i = 0; i < UINT16_MAX; i++)
        {
            if (is_prime(i))
            {
                print_dec(i);
                newline();
            }
        }
    }

    return 0;
}
