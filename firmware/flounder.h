#ifndef FLOUNDER_H
#define FLOUNDER_H

#include <stdbool.h>
#include <stdint.h>

#define SYSTEM_NAME "Flounder Z180"

// Memory
#define MEM(address) (*(volatile unsigned char *)(address))

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
