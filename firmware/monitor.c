#include "flounder.h"
#include <string.h>

void memdump(uint16_t start, uint16_t bytes)
{
    uint16_t i = 0;
    uint8_t b = 0;

    print_hex(start);
    print("  ");

    while (i < bytes)
    {
        b = peek(start + i);
        print_hex(b);
        putc(' ');

        i++;

        if (i % 16 == 0 && i < bytes)
        {
            print("|\r\n");
            print_hex(start + i);
            print("  ");
        }
        else if (i % 8 == 0)
        {
            putc(' ');
        }
    }

    putc('|');
}

int main()
{
    char buffer[64] = {0};

    init_flounder();

    newline();
    print("** ");
    print(SYSTEM_NAME);
    print(" System Monitor **");
    newline();

    while (true)
    {
        print("> ");

        readline(buffer, true);

        newline();

        if (strncmp(buffer, "dump", 4) == 0)
        {
            memdump(0, 256);
        }
        else
        {
            print("Command not implemented: ");
            print(buffer);
        }

        newline();
    }

    return 0;
}