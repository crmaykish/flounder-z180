#include <stdlib.h>
#include <string.h>
#include <z180.h>
#include "flounder.h"

static uint8_t counter = 0;

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

void cpu_init(void)
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

