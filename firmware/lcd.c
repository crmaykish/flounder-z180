#include "lcd.h"
#include <string.h>
#include <stdlib.h>
#include <z180.h>

// TODO: pass in a pointer to the buffer instead of allocating it statically here
uint8_t lcd_row = 0;
uint8_t lcd_col = 0;
char lcd_buffer[4][40] = {{0}};

void lcd_init()
{
    lcd_row = 0;
    lcd_col = 0;

    lcd_busy_wait(0);
    z180_outp(LCD_COMMAND0, LCD_COMMAND_SETMODE);

    lcd_busy_wait(1);
    z180_outp(LCD_COMMAND1, LCD_COMMAND_SETMODE);

    lcd_clear();
}

void lcd_busy_wait(uint8_t controller)
{
    while ((z180_inp((controller == 0 ? LCD_COMMAND0 : LCD_COMMAND1)) & LCD_BUSY_FLAG) != 0)
    {
    }
}

void lcd_putc(char c)
{
    uint8_t start_row = lcd_row;

    if (c == '\r')
    {
        // Return
        lcd_col = 0;

        if (lcd_row < 2)
        {
            z180_outp(LCD_COMMAND0, LCD_COMMAND_CURSOR_HOME);
        }
        else
        {
            z180_outp(LCD_COMMAND1, LCD_COMMAND_CURSOR_HOME);
        }
    }
    else if (c == '\n')
    {
        // New line
        lcd_row++;
    }
    else if (c == 0x08)
    {
        // TODO: Backspace
    }
    else if (c >= 0x20 && c < 0x7F)
    {
        // Printable character

        lcd_buffer[lcd_row][lcd_col] = c;

        if (lcd_row < 2)
        {
            lcd_busy_wait(0);
            z180_outp(LCD_DATA0, c);
        }
        else
        {
            lcd_busy_wait(1);
            z180_outp(LCD_DATA1, c);
        }

        // Increment LCD buffer index

        lcd_col++;

        if (lcd_col == 40)
        {
            // Wrap to the next row
            lcd_col = 0;
            lcd_row++;
        }
    }

    // If the row has changed, adjust accordingly
    if (start_row != lcd_row)
    {
        if (lcd_row == 1)
        {
            // Move up a row on the first controller
            lcd_busy_wait(0);
            z180_outp(LCD_COMMAND0, 0b10000000 + 40);
        }
        else if (lcd_row == 2)
        {
            // Switch from the first controller to the second
            lcd_busy_wait(0);
            z180_outp(LCD_COMMAND0, LCD_DISPLAY_COMMAND | LCD_DISPLAY_ON);

            lcd_busy_wait(1);
            z180_outp(LCD_COMMAND1, LCD_DISPLAY_COMMAND | LCD_DISPLAY_ON | LCD_CURSOR_ON | LCD_CURSOR_BLINK);

            // Make sure the second controller is on the right column (most likely 0)
            lcd_busy_wait(1);
            z180_outp(LCD_COMMAND1, 0b10000000 + lcd_col);
        }
        else if (lcd_row == 3)
        {
            // Move up a row on the second controller
            lcd_busy_wait(1);
            z180_outp(LCD_COMMAND1, 0b10000000 + 40);
        }
        else if (lcd_row == 4)
        {
            // Shift the LCD display contents up one row and continue on the last row

            // TODO: memccpy() would be slightly more efficient
            memcpy(lcd_buffer[0], lcd_buffer[1], 40);
            memcpy(lcd_buffer[1], lcd_buffer[2], 40);
            memcpy(lcd_buffer[2], lcd_buffer[3], 40);
            memset(lcd_buffer[3], 0, 40);

            // TODO: this is 90% of what lcd_clear does() just without clearing the buffers

            // Clear the display
            lcd_busy_wait(0);
            z180_outp(LCD_COMMAND0, LCD_COMMAND_CLEAR_DISPLAY);
            lcd_busy_wait(1);
            z180_outp(LCD_COMMAND1, LCD_COMMAND_CLEAR_DISPLAY);

            // reset cursors
            lcd_busy_wait(0);
            z180_outp(LCD_COMMAND0, LCD_COMMAND_CURSOR_HOME);
            lcd_busy_wait(1);
            z180_outp(LCD_COMMAND1, LCD_COMMAND_CURSOR_HOME);

            // Rewrite each of the rows to the row above it

            for (int i = 0; i < 40; i++)
            {
                if (lcd_buffer[0][i] != 0)
                {
                    lcd_busy_wait(0);
                    z180_outp(LCD_DATA0, lcd_buffer[0][i]);
                }
            }

            lcd_busy_wait(0);
            z180_outp(LCD_COMMAND0, 0b10000000 + 40);

            for (int i = 0; i < 40; i++)
            {
                if (lcd_buffer[1][i] != 0)
                {
                    lcd_busy_wait(0);
                    z180_outp(LCD_DATA0, lcd_buffer[1][i]);
                }
            }

            for (int i = 0; i < 40; i++)
            {
                if (lcd_buffer[2][i] != 0)
                {
                    lcd_busy_wait(1);
                    z180_outp(LCD_DATA1, lcd_buffer[2][i]);
                }
            }

            lcd_row = 3;
            lcd_col = 0;

            lcd_busy_wait(1);
            z180_outp(LCD_COMMAND1, 0b10000000 + 40);
        }
    }
}

void lcd_print(char *s)
{
    char i = 0;
    char c = s[i];

    while (c != 0)
    {
        lcd_putc(c);
        i++;
        c = s[i];
    }
}

void lcd_clear()
{
    lcd_busy_wait(0);
    z180_outp(LCD_COMMAND0, LCD_COMMAND_CLEAR_DISPLAY);
    lcd_busy_wait(0);
    z180_outp(LCD_COMMAND0, LCD_COMMAND_CURSOR_HOME);
    lcd_busy_wait(0);
    z180_outp(LCD_COMMAND0, LCD_DISPLAY_COMMAND | LCD_DISPLAY_ON | LCD_CURSOR_ON | LCD_CURSOR_BLINK);

    lcd_busy_wait(1);
    z180_outp(LCD_COMMAND1, LCD_COMMAND_CLEAR_DISPLAY);
    lcd_busy_wait(1);
    z180_outp(LCD_COMMAND1, LCD_COMMAND_CURSOR_HOME);
    lcd_busy_wait(1);
    z180_outp(LCD_COMMAND1, LCD_DISPLAY_COMMAND | LCD_DISPLAY_ON);

    lcd_row = 0;
    lcd_col = 0;

    // Clear the local LCD buffer
    memset(lcd_buffer, 0, 4 * 40);
}

uint8_t get_address()
{
    lcd_busy_wait(0);
    return z180_inp(LCD_COMMAND0 & LCD_ADDR_MASK);
}

void lcd_print_hex(uint32_t n)
{
    char buffer[11] = {0};

    if (n < 0x10)
    {
        lcd_putc('0');
    }

    utoa((unsigned int)n, buffer, 16);
    lcd_print(buffer);
}