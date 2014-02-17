#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h"
#include "leds.h"

void onfor(int led, int r, int g, int b, useconds_t usecs);

void usage(char *prog)
{
    printf("Usage: %s {delay in micro seconds}\n", prog);
    exit(-1);
}

int main(int argc, char **argv)
{
    long ldelay;
    useconds_t delay;

    if (argc != 2) usage(argv[0]);

    ldelay = atol(argv[1]);
    if (ldelay <= 0) usage(argv[0]);
    delay = (useconds_t) ldelay;

    if (!led_init()) exit(-1);

    while (1) {
        onfor(0, 1, 0, 0, delay);
        onfor(1, 1, 1, 0, delay);
        onfor(2, 0, 1, 0, delay);
        onfor(3, 0, 0, 1, delay);
        onfor(4, 1, 0, 1, delay);
    }

    return 0;
}

void onfor(int led, int r, int g, int b, useconds_t usecs)
{
    led_on(led, r, g, b);
    usleep(usecs);
    led_off(led);
}


