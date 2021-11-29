#include "asci.h"

void print(char *s)
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

int main()
{
    char *computer_name = "\r\n| === Flounder Z180 === |\r\n";
    char a = '!';

    asci1_init();

    print(computer_name);

    while (1)
    {
        asci1_putc(a);

        a++;

        if (a > '~')
        {
            a = '!';

            asci1_putc(0x0A);
            asci1_putc(0x0D);
        }
    }

    return 0;
}
