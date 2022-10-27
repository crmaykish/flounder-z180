#ifndef LCD_H
#define LCD_H

#include <stdint.h>

#define LCD_COMMAND0 0x6000
#define LCD_DATA0 0x6001
#define LCD_COMMAND1 0x8000
#define LCD_DATA1 0x8001

#define LCD_COMMAND_SETMODE 0x3F
#define LCD_COMMAND_CURSOR_ON 0x0F
#define LCD_COMMAND_CLEAR_DISPLAY 1
#define LCD_COMMAND_CURSOR_HOME 2

#define LCD_BUSY_FLAG 0b10000000
#define LCD_ADDR_MASK 0b01111111

#define LCD_DISPLAY_COMMAND 0b00001000
#define LCD_DISPLAY_ON 0b100
#define LCD_CURSOR_ON 0b10
#define LCD_CURSOR_BLINK 0b1

void lcd_init();
void lcd_busy_wait(uint8_t controller);
void lcd_putc(char c);
void lcd_print(char *s);
void lcd_clear();
void lcd_print_hex(uint32_t n);
uint8_t get_address();

#endif