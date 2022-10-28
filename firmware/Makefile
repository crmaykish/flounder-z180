CC = zcc

CFLAGS = -v +z180 -SO3 -clib=sdcc_iy -g -m

ROMPRAGMAS = -pragma-define:CRT_INTERRUPT_MODE=2 \
             -pragma-define:CRT_ENABLE_EIDI=0x13 \
             -pragma-define:CRT_ORG_VECTOR_TABLE=0xFB00 \
             -pragma-define:CRT_ORG_CODE=0 \
             -pragma-define:CRT_INCLUDE_PREAMBLE=0

MONITOR_SRC = monitor.c flounder.c asci.c lcd.c ps2.c run.asm

HELLO_SRC = hello.c flounder.c asci.c lcd.c

.PHONY: all clean

all: monitor hello

monitor: $(MONITOR_SRC)
	$(CC) $(CFLAGS) -startup=1 -o monitor -create-app $(MONITOR_SRC) $(ROMPRAGMAS)

hello: $(HELLO_SRC)
	$(CC) $(CFLAGS) -startup=-1 -o hello.bin $(HELLO_SRC)

clean:
	@$(RM) *.bin *.rom *.def *.map monitor hello
