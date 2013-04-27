
all: dumpgpio rainbow shades knightrider reset coloursweep spectrum

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

reset: gpio.o leds.o reset.o
	gcc -Wall -Wextra $^ -o $@

%.o: %.c
	gcc -c $^ -Wall -Wextra -fPIC -fno-inline -g -o $@

clean:
	rm -f *.o *~ core.*
