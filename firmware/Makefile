CC = zcc

CFLAGS = -v +z180 -startup=1 -SO3 -clib=sdcc_iy -g -m -create-app

PRAGMAS = -pragma-define:CRT_INTERRUPT_MODE=2 \
          -pragma-define:CRT_ENABLE_EIDI=0x13 \
          -pragma-define:CRT_ORG_VECTOR_TABLE=0xFB00 \
          -pragma-define:CRT_ORG_CODE=0 \
          -pragma-define:CRT_INCLUDE_PREAMBLE=0

MONITOR_SRC = monitor.c flounder.c asci.c lcd.c ps2.c

.PHONY: all clean

all: monitor

monitor: $(MONITOR_SRC)
	$(CC) $(CFLAGS) -o monitor $(MONITOR_SRC) $(PRAGMAS)

clean:
	@$(RM) *.bin *.rom *.def *.map monitor
