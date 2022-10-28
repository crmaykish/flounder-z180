#include <z180.h>
#include "flounder.h"
#include "asci.h"
#include "lcd.h"

int main()
{
    unsigned char a = 0;
    int b = 0;

    uart_print("Blinking some LEDs...\r\n");
    lcd_clear();

    while (1)
    {
        uart_print_dec(b);
        uart_print("\r\n");

        lcd_print_hex(b);
        lcd_print("\r\n");

        b++;

        z180_outp(0x4001, a);
        a++;

        z180_delay_ms(250);
    }

    return 0;
}
