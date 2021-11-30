#include <stdlib.h>
#include "flounder.h"

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

int main()
{
    uint16_t count = 1;

    init_flounder();

    newline();
    print(SYSTEM_NAME);
    newline();

    while (1)
    {
        for (uint16_t i = 1; i < UINT16_MAX; i++)
        {
            if (is_prime(i))
            {
                print_dec(count);
                print(": ");
                print_dec(i);
                newline();
                count++;
            }
        }
    }

    return 0;
}
