#ifndef FLOUNDER_H
#define FLOUNDER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define SYSTEM_NAME "Flounder Z180"

// TODO: why don't the internal registers defined in z180.h work?
#define CNTLA0 0x0000
#define CNTLA1 0x0001
#define CNTLB0 0x0002
#define CNTLB1 0x0003
#define STAT0 0x0004
#define STAT1 0x0005
#define TDR0 0x0006
#define TDR1 0x0007
#define RDR0 0x0008
#define RDR1 0x0009

#define ASEXT0 0x0012
#define ASEXT1 0x0013

#define CCR 0x001F
#define DCNTL 0x0032
#define IL 0x0033
#define ITC 0x0034
#define OMCR 0x003E

#define TMDR0L 0x000C
#define TMDR0H 0x000D
#define RLDR0L 0x000E
#define RLDR0H 0x000F
#define TCR 0x0010
#define TMDR1L 0x0014
#define TMDR1H 0x0015
#define RLDR1L 0x0016
#define RLDR1H 0x0017

// PIO
#define PORTA_DATA 0x2000
#define PORTB_DATA 0x2001
#define PORTA_CMD 0x2002
#define PORTB_CMD 0x2003

// CPLD
#define CPLD0 0x4000

// LCD Display
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

// Memory
#define MEM(address) (*(volatile uint8_t *)(address))

// ASCI
void asci0_putc(char);
char asci0_getc();

// Hardware
void flounder_init(void);

// LCD Display
void lcd_init();
void lcd_busy_wait(uint8_t controller);
void lcd_putc(char c);
void lcd_print(char *s);
void lcd_clear();
uint8_t get_address();

void lcd_print_hex(uint32_t n);

// I/O Helpers
void uart_print(char *);
void uart_print_dec(uint32_t);
void uart_print_hex(uint32_t);
uint16_t uart_readline(char *buffer, bool echo);

// Utilities
char *parse_param(char *s, char delim, size_t n);

#endif
