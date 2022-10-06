#ifndef FLOUNDER_H
#define FLOUNDER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define SYSTEM_NAME "Flounder Z180"

// Memory
#define MEM(address) (*(volatile uint8_t *)(address))

// ASCI
extern void asci1_init();
extern void asci1_putc(char);
extern char asci1_getc();

// CPLD
extern uint8_t cpld_read();

// PIO
extern void pio_init();
extern void pio_setb(uint8_t val);

// Hardware
void flounder_init(void);

// I/O Helpers
void uart_print(char *);
void uart_print_dec(uint32_t);
void uart_print_hex(uint32_t);
uint16_t uart_readline(char *buffer, bool echo);

// Utilities
char *parse_param(char *s, char delim, size_t n);

#endif
