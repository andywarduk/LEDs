DEBUG=
# Uncomment to build with DEBUG then make clean, make
# DEBUG=-DDEBUG

TARGETS=dumpgpio rainbow shades knightrider reset coloursweep spectrum progress rate

all: $(TARGETS)

install: $(TARGETS)
	cp $(TARGETS) /usr/local/bin

dumpgpio: gpio.o dumpgpio.o
	gcc -Wall -Wextra $^ -o $@

rainbow: gpio.o leds.o rainbow.o
	gcc -Wall -Wextra $^ -o $@

shades: gpio.o leds.o shades.o
	gcc -Wall -Wextra $^ -o $@

knightrider: gpio.o leds.o knightrider.o
	gcc -Wall -Wextra $^ -o $@

coloursweep: gpio.o leds.o coloursweep.o
	gcc -Wall -Wextra $^ -o $@

spectrum: gpio.o leds.o spectrum.o
	gcc -Wall -Wextra $^ -o $@

progress: gpio.o leds.o progress.o
	gcc -Wall -Wextra $^ -o $@

rate: gpio.o leds.o rate.o
	gcc -Wall -Wextra $^ -o $@

reset: gpio.o leds.o reset.o
	gcc -Wall -Wextra $^ -o $@

%.o: %.c
	gcc $(DEBUG) -c $^ -Wall -Wextra -fPIC -fno-inline -g -o $@

clean:
	rm -f *.o *~ core.*
