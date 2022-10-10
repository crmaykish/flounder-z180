#include <stdlib.h>
#include <string.h>
#include <z180.h>
#include "flounder.h"

static uint8_t counter = 0;
uint8_t lcd_row = 0;
uint8_t lcd_col = 0;
char lcd_buffer[4][40] = {{0}};

// void z180_int_asci0() __critical __interrupt(0x0E)
// {
//     // TODO: ASCI0 ISR
//     asci0_putc('0');
// }

// void z180_int_asci1() __critical __interrupt(0x10)
// {
//     asci0_putc('1');
// }

void my_isr() __critical __interrupt(0x04)
{
    // Clear the interrupt by reading these registers
    uint8_t a = z180_inp(TCR);
    uint8_t b = z180_inp(TMDR0L);

    counter++;
    z180_outp(PORTB_DATA, counter);
}

void bad_interrupt() __critical __interrupt(0)
{
    uart_print("\r\nBAD\r\n");
    while (1)
    {
    }
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

void flounder_init(void)
{
    interrupts_disable();

    z180_outp(STAT0, 0x00); // disable interrupts on ASCI0
    z180_outp(STAT1, 0x00); // disable interrupts on ASCI1

    // Disable external interrupts
    z180_outp(ITC, 0x00);

    // Assuming CLK oscillator is 18.432 MHz

    // Set PHI to CLK / 2 = 9.216 MHz
    z180_outp(CCR, 0x00);

    // Set 3 memory wait states and 4 I/O wait states
    z180_outp(DCNTL, 0b11110000);

    // Set CPU mode to full Z80 compatibility
    z180_outp(OMCR, 0);

    // Set X1 bit in ASEXT to 0 for 16/64 ASCI clock divider
    // BRG mode to 0 for PHI/10 or PHI/30
    z180_outp(ASEXT0, 0b00000000);

    // Set ASCI0 baudrate to: PHI / 10 / 16 / 1 = 57600 baud (where PHI is 9.216 MHz)
    z180_outp(CNTLB0, 0b00000000);

    // Set Transmit and Receive Enable ON for ASCI 1, 8-bit, no parity, 1 stop bit
    z180_outp(CNTLA0, 0b01110100);

    // Set PIO port B to output mode
    z180_outp(PORTB_CMD, 0b00001111);

    // Set PIO port B to zero
    z180_outp(PORTB_DATA, 0x00);

    for (uint16_t i = 0xFB00; i < 0xFC00; i += 2)
    {
        z180_wpoke(i, (uint16_t)bad_interrupt);
    }

    // Add timer ISR to vector table
    z180_wpoke(0xFB04, (uint16_t)my_isr);

    // Load timer 0 with 0x1000 starting value  (roughly 9 ticks per second)
    z180_outp(RLDR0H, 0xC0);
    z180_outp(RLDR0L, 0x00);

    // Enable timer 0 interrupts and start timer 0 counting
    z180_outp(TCR, 0b00010001);

    lcd_init();

    interrupts_enable();
}

void uart_print(char *s)
{
    char i = 0;
    char c = s[i];

    while (c != 0)
    {
        asci0_putc(c);
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
        asci0_putc('0');
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
        in = asci0_getc();

        if (echo)
        {
            asci0_putc(in);
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