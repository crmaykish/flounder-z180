#include <stdlib.h>
#include <string.h>
#include <z180.h>
#include "flounder.h"
#include "asci.h"
#include "lcd.h"
#include "ps2.h"

void print_binary_string(char *str, uint8_t max);
void memdump(uint16_t address, uint16_t bytes);
void load(uint16_t addr);

int main()
{
    char buffer[32];
    char c = 0;
    uint8_t i = 0;
    bool newline = true;

    cpu_init();
    lcd_init();

    lcd_print("Flounder Z180\r\n");

    while (true)
    {
        // TODO: first command after reset is always bad, why?

        memset(buffer, 0, sizeof(buffer));
        c = 0;
        i = 0;
        newline = true;

        lcd_print("~");

        // Read characters from PS/2 keyboard
        while (c != '\r' && i < sizeof(buffer))
        {
            c = ps2_get_char();
            lcd_putc(c);
            buffer[i] = c;
            i++;
        }

        lcd_print("\r\n");

        if (strncmp(buffer, "help", 4) == 0)
        {
            lcd_print("help peek poke ipeek ipoke dump clear");
        }

        else if (strncmp(buffer, "dump", 4) == 0)
        {
            char *param = parse_param(buffer, ' ', sizeof(buffer));

            if (param == NULL)
            {
                break;
            }

            uint16_t addr = (uint16_t)strtoul(param, 0, 16);
            memdump(addr, 24);
        }

        else if (strncmp(buffer, "peek", 4) == 0)
        {
            char *param = parse_param(buffer, ' ', sizeof(buffer));

            if (param == NULL)
            {
                break;
            }

            uint16_t addr = (uint16_t)strtoul(param, 0, 16);

            lcd_print_hex(z180_bpeek(addr));
        }

        else if (strncmp(buffer, "poke", 4) == 0)
        {
            char *param1 = parse_param(buffer, ' ', sizeof(buffer));
            char *param2 = parse_param(&(buffer[strnlen(param1, 20)]), ' ', 20); // TODO: HACK!!! properly check the string length

            if (param1 == NULL || param2 == NULL)
            {
                break;
            }

            uint16_t addr = (uint16_t)strtoul(param1, 0, 16);
            uint8_t val = (uint8_t)strtoul(param2, 0, 16);

            z180_bpoke(addr, val);

            newline = false;
        }

        else if (strncmp(buffer, "ipeek", 5) == 0)
        {
            char *param = parse_param(buffer, ' ', sizeof(buffer));

            if (param == NULL)
            {
                break;
            }

            uint16_t addr = (uint16_t)strtoul(param, 0, 16);

            lcd_print_hex(z180_inp(addr));
        }

        else if (strncmp(buffer, "ipoke", 5) == 0)
        {
            char *param1 = parse_param(buffer, ' ', sizeof(buffer));
            char *param2 = parse_param(&(buffer[strnlen(param1, 20)]), ' ', 20); // TODO: HACK!!! properly check the string length

            if (param1 == NULL || param2 == NULL)
            {
                break;
            }

            uint16_t addr = (uint16_t)strtoul(param1, 0, 16);
            uint8_t val = (uint8_t)strtoul(param2, 0, 16);

            z180_outp(addr, val);

            newline = false;
        }

        // else if (strncmp(buffer, "load", 4) == 0)
        // {
        //     char *param = parse_param(buffer, ' ', sizeof(buffer));

        //     if (param == NULL)
        //     {
        //         break;
        //     }

        //     uint16_t addr = (uint16_t)strtoul(param, 0, 16);

        //     load(addr);
        // }

        // else if (strncmp(buffer, "run", 3) == 0)
        // {
        //     lcd_print("Jumping to 0xB000...\r\n");
        //     run();
        // }

        else if (strncmp(buffer, "clear", 5) == 0)
        {
            lcd_clear();
            newline = false;
        }

        else
        {
            lcd_print("Bad command: ");
            lcd_print(buffer);
        }

        if (newline)
        {
            lcd_print("\r\n");
        }
    }

    return 0;
}

void memdump(uint16_t address, uint16_t bytes)
{
    uint16_t i = 0;
    uint16_t b = 0;

    lcd_print_hex(address);
    lcd_print("  ");

    while (i < bytes)
    {
        b = MEM(address + i);
        lcd_print_hex(b);
        lcd_putc(' ');

        i++;

        if (i % 8 == 0 && i < bytes)
        {
            lcd_print("\r\n");
            lcd_print_hex(address + i);
            lcd_print("  ");
        }
    }
}

void load(uint16_t addr)
{
    uint16_t in_count = 0;
    uint8_t magic_count = 0;
    uint8_t in = 0;

    lcd_print("Loading into 0x");
    lcd_print_hex(addr);
    lcd_print("...");

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

    lcd_print("Done!");
}
