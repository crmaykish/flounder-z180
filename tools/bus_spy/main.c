#include <stdio.h>
#include "pico/stdlib.h"
#include <stdint.h>

#define ADDR_BUS_MASK 0x0FFF
#define DATA_BUS_MASK 0xFF000

#define RESET_PIN 20
#define EXTAL_PIN 21
#define IORQ_PIN 22
#define MREQ_PIN 26
#define WR_PIN 27
#define RD_PIN 28

#define ADDR_WIDTH 12

#define MEMORY_SIZE 4096

#define CLK_DELAY 50

void font_red()
{
    printf("\033[31m");
}

void font_green()
{
    printf("\033[32m");
}

void font_reset()
{
    printf("\033[0m");
}

// Memory
static uint8_t memory[MEMORY_SIZE] = {0xed, 0x38, 0x1f, 0x3e, 0x80, 0xed, 0x39, 0x1f, 0xc3, 0x00, 0x00};

void cycle(int delay)
{
    gpio_put(EXTAL_PIN, true);
    sleep_ms(delay);
    gpio_put(EXTAL_PIN, false);
    sleep_ms(delay);
}

int main()
{
    stdio_init_all();

    // Set all the GPIO pins to inputs
    gpio_init_mask(0xFFFFFFFF);
    gpio_set_dir_in_masked(0xFFFFFFFF);

    // Setup output pins
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_set_dir(RESET_PIN, GPIO_OUT);
    gpio_set_dir(EXTAL_PIN, GPIO_OUT);

    // Turn onboard LED on
    gpio_put(PICO_DEFAULT_LED_PIN, true);

    // Reset the Z180
    gpio_put(RESET_PIN, false);
    cycle(CLK_DELAY);
    cycle(CLK_DELAY);
    cycle(CLK_DELAY);
    cycle(CLK_DELAY);
    cycle(CLK_DELAY);
    cycle(CLK_DELAY);
    cycle(CLK_DELAY);
    cycle(CLK_DELAY);
    cycle(CLK_DELAY);
    cycle(CLK_DELAY);
    gpio_put(RESET_PIN, true);

    sleep_ms(CLK_DELAY);

    uint16_t addr_bus = 0;
    uint8_t data_bus = 0;
    bool mreq = 0;
    bool ioreq = 0;
    bool rd = 0;
    bool wr = 0;
    uint32_t cycle = 1;

    uint32_t io_pins;

    while (true)
    {
        gpio_put(EXTAL_PIN, true);
        sleep_ms(CLK_DELAY);

        io_pins = gpio_get_all();

        addr_bus = io_pins & ADDR_BUS_MASK;
        data_bus = (io_pins & DATA_BUS_MASK) >> ADDR_WIDTH;
        mreq = gpio_get(MREQ_PIN);
        ioreq = gpio_get(IORQ_PIN);
        rd = gpio_get(RD_PIN);
        wr = gpio_get(WR_PIN);

        if (!mreq)
        {
            if (!rd)
            {
                gpio_set_dir_out_masked(DATA_BUS_MASK);
                gpio_put_masked(DATA_BUS_MASK, (memory[addr_bus] << ADDR_WIDTH));

                printf("RD: %02X from %04X\r\n", memory[addr_bus], addr_bus);
            }
            if (!wr)
            {
                memory[addr_bus] = data_bus;

                printf("WR: %02X to %04X\r\n", data_bus, addr_bus);
            }
        }
        else
        {
            gpio_set_dir_in_masked(DATA_BUS_MASK);
        }
        
        if (!ioreq)
        {
            if (!rd)
            {
                font_red();
                printf("Read IO: %02X from %04X\r\n", data_bus, addr_bus);
                font_reset();
            }
            if (!wr)
            {
                font_green();
                printf("Write IO: %02X to %04X\r\n", data_bus, addr_bus);
                font_reset();
            }
        }

        printf("%u | ADDR: %04X | DATA: %02X | MREQ: %d | IOREQ: %d | RD: %d | WR: %d\r\n", cycle, addr_bus, data_bus, mreq, ioreq, rd, wr);

        gpio_put(EXTAL_PIN, false);
        sleep_ms(CLK_DELAY);

        cycle++;
    }

    return 0;
}
