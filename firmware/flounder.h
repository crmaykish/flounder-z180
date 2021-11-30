#ifndef FLOUNDER_H
#define FLOUNDER_H

#include <stdbool.h>
#include <stdint.h>

#define SYSTEM_NAME "Flounder Z180"

void init_flounder();

void print(char *);
void print_dec(uint32_t);
void print_hex(uint32_t);
void newline();

#endif