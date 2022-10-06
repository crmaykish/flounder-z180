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
#define OMCR 0x003E

// PIO
#define PORTA_DATA 0x2000
#define PORTB_DATA 0x2001
#define PORTA_CMD 0x2002
#define PORTB_CMD 0x2003

// CPLD
#define CPLD0 0x4000

// Memory
#define MEM(address) (*(volatile uint8_t *)(address))

// ASCI
void asci0_putc(char);
char asci0_getc();

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
