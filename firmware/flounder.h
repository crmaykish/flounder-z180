#ifndef FLOUNDER_H
#define FLOUNDER_H

#include <stdbool.h>
#include <stdint.h>

#define SYSTEM_NAME "Flounder Z180"

// System
void init_flounder();
uint8_t peek(uint16_t addr);
void poke(uint16_t addr, uint8_t val);

// UART
void print(char *);
void print_dec(uint32_t);
void print_hex(uint32_t);
void newline();
uint16_t readline(char *buffer, bool echo);

#endif