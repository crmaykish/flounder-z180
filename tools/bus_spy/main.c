#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include <stdint.h>

#define ADDR_BUS_MASK 0x0FFF
#define DATA_BUS_MASK 0xFF000

#define RESET_PIN 20
#define EXTAL_PIN 21
#define MREQ_PIN 26
#define RD_PIN 28

#define ADDR_WIDTH 12

#define MEMORY_SIZE 4096

#define CLK_DELAY_US 5 // 100 kHz

void clock_sim()
{
    while (true)
    {
        gpio_put(EXTAL_PIN, true);
        sleep_us(CLK_DELAY_US);
        gpio_put(EXTAL_PIN, false);
        sleep_us(CLK_DELAY_US);
    }
}

// Memory
static uint8_t memory[MEMORY_SIZE] = {
    0x3E, 0x00, 0xED, 0x39, 0x3E, 0x3E, 0x00, 0xED,
    0x39, 0x32, 0x3E, 0x07, 0xED, 0x39, 0x03, 0x3E,
    0x00, 0xED, 0x39, 0x13, 0x3E, 0x64, 0xED, 0x39,
    0x01, 0xED, 0x38, 0x05, 0xCB, 0x4F, 0xCA, 0x19,
    0x00, 0x3E, 0x0A, 0xED, 0x39, 0x07, 0xED, 0x38,
    0x05, 0xCB, 0x4F, 0xCA, 0x26, 0x00, 0x3E, 0x0D,
    0xED, 0x39, 0x07, 0x06, 0x41, 0xED, 0x38, 0x05,
    0xCB, 0x4F, 0xCA, 0x35, 0x00, 0x78, 0xED, 0x39,
    0x07, 0x04, 0x78, 0xFE, 0x5B, 0xCA, 0x19, 0x00,
    0xC3, 0x35, 0x00};

uint16_t addr_bus = 0;
uint8_t data_bus = 0;
bool mreq = 0;
bool ioreq = 0;
bool rd = 0;
bool wr = 0;

uint32_t io_pins;

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

    multicore_launch_core1(clock_sim);

    // Turn onboard LED on
    gpio_put(PICO_DEFAULT_LED_PIN, true);

    // Reset the Z180
    gpio_put(RESET_PIN, false);
    sleep_ms(250);
    gpio_put(RESET_PIN, true);

    while (true)
    {
        io_pins = gpio_get_all();

        addr_bus = io_pins & ADDR_BUS_MASK;
        data_bus = (io_pins & DATA_BUS_MASK) >> ADDR_WIDTH;
        mreq = gpio_get(MREQ_PIN);
        rd = gpio_get(RD_PIN);

        if (!mreq & !rd)
        {
            gpio_set_dir_out_masked(DATA_BUS_MASK);
            gpio_put_masked(DATA_BUS_MASK, (memory[addr_bus] << ADDR_WIDTH));
        }
        else
        {
            gpio_set_dir_in_masked(DATA_BUS_MASK);
        }
    }

    return 0;
}
