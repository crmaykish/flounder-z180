#include <z180.h>
#include "ps2.h"
#include "flounder.h"

char ps2_scan_code_to_ascii(unsigned char code)
{
    char a = 0;

    switch (code)
    {
    case 0x5A:
        a = '\r';
        break;
    case 0x29:
        a = ' ';
        break;
    case 0x16:
        a = '1';
        break;
    case 0x1E:
        a = '2';
        break;
    case 0x26:
        a = '3';
        break;
    case 0x25:
        a = '4';
        break;
    case 0x2E:
        a = '5';
        break;
    case 0x36:
        a = '6';
        break;
    case 0x3D:
        a = '7';
        break;
    case 0x3E:
        a = '8';
        break;
    case 0x46:
        a = '9';
        break;
    case 0x45:
        a = '0';
        break;
    case 0x1C:
        a = 'a';
        break;
    case 0x32:
        a = 'b';
        break;
    case 0x21:
        a = 'c';
        break;
    case 0x23:
        a = 'd';
        break;
    case 0x24:
        a = 'e';
        break;
    case 0x2B:
        a = 'f';
        break;
    case 0x34:
        a = 'g';
        break;
    case 0x33:
        a = 'h';
        break;
    case 0x43:
        a = 'i';
        break;
    case 0x3B:
        a = 'j';
        break;
    case 0x42:
        a = 'k';
        break;
    case 0x4B:
        a = 'l';
        break;
    case 0x3A:
        a = 'm';
        break;
    case 0x31:
        a = 'n';
        break;
    case 0x44:
        a = 'o';
        break;
    case 0x4D:
        a = 'p';
        break;
    case 0x15:
        a = 'q';
        break;
    case 0x2D:
        a = 'r';
        break;
    case 0x1B:
        a = 's';
        break;
    case 0x2C:
        a = 't';
        break;
    case 0x3C:
        a = 'u';
        break;
    case 0x2A:
        a = 'v';
        break;
    case 0x1D:
        a = 'w';
        break;
    case 0x22:
        a = 'x';
        break;
    case 0x35:
        a = 'y';
        break;
    case 0x1A:
        a = 'z';
        break;
    default:
        a = 0;
    }

    return a;
}

bool ps2_char_available()
{
    return (z180_inp(CPLD_PS2) != 0);
}

char ps2_get_char()
{
    bool char_available = false;
    bool break_char = false;
    char a = 0;

    while (!char_available)
    {
        a = z180_inp(CPLD_PS2);

        if (a != 0)
        {
            // Acknowledge that the scancode was received (this clears the buffer on the CPLD)
            z180_outp(CPLD_PS2, 0);

            if (a == PS2_BREAK_BYTE)
            {
                break_char = true;
            }
            else
            {
                if (break_char)
                {
                    break_char = false;
                }
                else
                {
                    char_available = true;
                }
            }
        }
    }

    // Convert the scancode to an ASCII character and return
    return ps2_scan_code_to_ascii(a);
}
