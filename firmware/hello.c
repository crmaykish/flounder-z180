#include <z180.h>
#include "flounder.h"
#include "asci.h"

int main()
{
    unsigned char a = 0;
    int b = 0;

    uart_print("Blinking some LEDs...\r\n");

    while (1)
    {
        uart_print_dec(b);
        uart_print("\r\n");
        b++;

        z180_outp(0x4001, a);
        a++;

        z180_delay_ms(250);
    }

    return 0;
}
