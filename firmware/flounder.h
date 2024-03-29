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

#define CMR 0x001E
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
#define CPLD_PS2 0x4000
#define CPLD_LED 0x4001

// Memory
#define MEM(address) (*(volatile uint8_t *)(address))

// Hardware
extern void run();

void cpu_init(void);

// Utilities
char *parse_param(char *s, char delim, size_t n);

#endif
