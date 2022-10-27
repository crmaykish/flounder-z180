#ifndef ASCI_H
#define ASCI_H

#include <stdint.h>
#include <stdbool.h>

void asci0_putc(char);
char asci0_getc();
void asci1_putc(char);
char asci1_getc();

void uart_print(char *);
void uart_print_dec(uint32_t);
void uart_print_hex(uint32_t);
uint16_t uart_readline(char *buffer, bool echo);

#endif
