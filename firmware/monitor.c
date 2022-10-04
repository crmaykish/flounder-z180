#include <stdlib.h>
#include <string.h>
#include "flounder.h"
#include "asci.h"
#include "ps2.h"

void print_binary_string(char *str, uint8_t max)
{
    uint8_t i = 0;

    while (i < max)
    {
        if (str[i] >= 32 && str[i] < 127)
        {
            asci1_putc(str[i]);
        }
        else
        {
            asci1_putc('.');
        }

        i++;
    }
}

void memdump(uint16_t address, uint16_t bytes)
{
    uint16_t i = 0;
    uint16_t b = 0;

    uart_print_hex(address);
    uart_print("  ");

    while (i < bytes)
    {
        b = MEM(address + i);
        uart_print_hex(b);
        uart_print(" ");

        i++;

        if (i % 16 == 0 && i < bytes)
        {
            uart_print(" |");
            print_binary_string((char *)(address + i - 16), 16);
            uart_print("|\r\n");
            uart_print_hex(address + i);
            uart_print("  ");
        }
        else if (i % 8 == 0)
        {
            uart_print(" ");
        }
    }

    uart_print("|");
    print_binary_string((char *)(address + i - 16), 16);
    uart_print("|");
}

void load(uint16_t addr)
{
    uint16_t in_count = 0;
    uint8_t magic_count = 0;
    uint8_t in = 0;

    uart_print("Loading into 0x");
    uart_print_hex(addr);
    uart_print("...");

    while (magic_count != 3)
    {
        in = asci1_getc();

        MEM(addr + in_count) = in;

        if (in == 0xDE)
        {
            magic_count++;
        }
        else
        {
            magic_count = 0;
        }

        in_count++;
    }

    // Remove the magic bytes from the end of the firmware in RAM
    memset((uint16_t *)(addr + in_count - 3), 0, 3);

    uart_print_dec(in_count - 3);
    uart_print(" bytes read\r\nDone!");
}

int main()
{
    char buffer[32] = {0};

    flounder_init();

    uart_print("\r\n");
    uart_print("** ");
    uart_print(SYSTEM_NAME);
    uart_print(" System Monitor **");
    uart_print("\r\n");

    uart_print("\r\n");

    while (true)
    {
        uart_print("\r\n> ");

        memset(buffer, 0, sizeof(buffer));

        uart_readline(buffer, true);

        uart_print("\r\n");

        if (strncmp(buffer, "dump", 4) == 0)
        {
            char *param = parse_param(buffer, ' ', sizeof(buffer));

            if (param == NULL)
            {
                break;
            }

            uint16_t addr = (uint16_t)strtoul(param, 0, 16);
            memdump(addr, 256);
        }
        else if (strncmp(buffer, "load", 4) == 0)
        {
            char *param = parse_param(buffer, ' ', sizeof(buffer));

            if (param == NULL)
            {
                break;
            }

            uint16_t addr = (uint16_t)strtoul(param, 0, 16);

            load(addr);
        }
        else if (strncmp(buffer, "cpld", 4) == 0)
        {
            char last_char = 0;

            uart_print("\r\n");
            while (true)
            {
                char a = ps2_get_char();

                if ((a != 0) && (a != last_char) && a != '*')
                {
                    last_char = a;
                    asci1_putc(a);
                }
            }
        }
        else
        {
            uart_print("Command not implemented: ");
            uart_print(buffer);
        }
    }

    return 0;
}
