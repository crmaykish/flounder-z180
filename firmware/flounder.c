#include <stdlib.h>
#include <z180.h>
#include "flounder.h"

void ISR_prt0()
{
    // Clear the interrupt by reading these registers
    uint8_t a = z180_inp(TCR);
    uint8_t b = z180_inp(TMDR0L);

    asci0_putc('x');

    // TODO: how do we change static variables from within an ISR?
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
    // Assuming CLK oscillator is 18.432 MHz

    // Set PHI to CLK / 2 = 9.216 MHz
    z180_outp(CCR, 0x00);

    // Set 0 memory wait states and 2 I/O wait states
    z180_outp(DCNTL, 0b00010000);

    // Set CPU mode to full Z80 compatibility
    z180_outp(OMCR, 0);

    // Set X1 bit in ASEXT to 0 for 16/64 ASCI clock divider
    // BRG mode to 0 for PHI/10 or PHI/30
    z180_outp(ASEXT0, 0b00000000);

    // Set ASCI0 baudrate to: PHI / 30 / 16 / 2 = 9600 baud (where PHI is 9.216 MHz)
    z180_outp(CNTLB0, 0b00100001); // set to PHI / 30 / 16 / 2

    // Set Transmit and Receive Enable ON for ASCI 1, 8-bit, no parity, 1 stop bit
    z180_outp(CNTLA0, 0b01110100);

    // Set PIO port B to output mode
    z180_outp(PORTB_CMD, 0b00001111);

    // Set PIO port B to zero
    z180_outp(PORTB_DATA, 0x00);

    // Disable external interrupts
    z180_outp(ITC, 0x00);

    // Load timer 0 with 0x1000 starting value  (roughly 9 ticks per second)
    z180_outp(RLDR0H, 0xC0);
    z180_outp(RLDR0L, 0x00);

    // Enable timer 0 interrupts and start timer 0 counting
    z180_outp(TCR, 0b00010001);

    lcd_init();
    lcd_print("Flounder Z180");
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
    lcd_busy_wait();
    z180_outp(LCD_COMMAND, LCD_COMMAND_SETMODE);
    lcd_busy_wait();
    z180_outp(LCD_COMMAND, LCD_COMMAND_CURSOR_ON);
    lcd_busy_wait();
    z180_outp(LCD_COMMAND, LCD_COMMAND_CLEAR_DISPLAY);
    lcd_busy_wait();
    z180_outp(LCD_COMMAND, LCD_COMMAND_CURSOR_HOME);
}

void lcd_busy_wait()
{
    while ((z180_inp(LCD_COMMAND) & 0b10000000) != 0)
    {
    }
}

void lcd_print(char *s)
{
    char i = 0;
    char c = s[i];

    while (c != 0)
    {
        lcd_busy_wait();
        z180_outp(LCD_DATA, c);
        i++;
        c = s[i];
    }
}
