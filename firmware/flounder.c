#include <stdlib.h>
#include <string.h>
#include <z180.h>
#include "flounder.h"

static uint8_t counter = 0;
uint8_t lcd_row = 0;
uint8_t lcd_col = 0;
char lcd_buffer[4][40] = {{0}};

void z180_int_asci0() __critical __interrupt(0x0E)
{
    // TODO: ASCI0 ISR
}

void z180_int_asci1() __critical __interrupt(0x10)
{
    // TODO: ASCI1 ISR
}

void z180_int_prt0() __critical __interrupt(0x04)
{
    // Clear the interrupt by reading these registers
    uint8_t a = z180_inp(TCR);
    uint8_t b = z180_inp(TMDR0L);

    counter++;
    z180_outp(PORTA_DATA, counter);
}

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

void flounder_init(void)
{
    // Set clock divide to XTAL/1 (i.e. run at the full oscillator frequency)
    z180_outp(CCR, 0x80);

    // Enable X2 clock multiplier (PHI runs at 2x oscillator)
    z180_outp(CMR, 0x80);

    // Set 1 memory wait states and 2 I/O wait state
    z180_outp(DCNTL, 0b01010000);

    // Set CPU mode to full Z80 compatibility
    z180_outp(OMCR, 0);

    // Set ASCI 1 to use external clock source, 57600 baud at 1.8432/2 MHz
    z180_outp(CNTLB1, 0b00000111);

    // Set Transmit and Receive Enable ON for ASCI 1, 8-bit, no parity, 1 stop bit
    z180_outp(CNTLA1, 0b01100100);

    // Set PIO Port A to output and zero it
    z180_outp(PORTA_CMD, 0b00001111);
    z180_outp(PORTA_DATA, 0x00);

    // Disable external interrupts
    z180_outp(ITC, 0x00);

    // Load timer 0 with 0x4000 starting value
    z180_outp(RLDR0H, 0x40);
    z180_outp(RLDR0L, 0x00);

    // Enable timer 0 interrupts and start timer 0 counting
    z180_outp(TCR, 0b00010001);

    lcd_init();
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
            lcd_putc(in);
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