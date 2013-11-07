#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h"
#include "leds.h"

#define QUANTA 5000
#define LEDS 5

void onfor(int led, int r, int g, int b, useconds_t usecs);


void Usage(char *prog)
{
    printf("Usage: %s <rate> <scale> <dispsecs>\n", prog);
    exit(1);
}

int main(int argc, char **argv)
{
    int rate;
    int scale;
    int dispsecs;

    int curled;
    unsigned int amt;
    unsigned int trigger;

    unsigned int i;

    // Get args
    if(argc != 4){
        Usage(argv[0]);
    }
    rate = atoi(argv[1]);
    scale = atoi(argv[2]);
    dispsecs = atoi(argv[3]);
    if(dispsecs <= 0){
        Usage(argv[0]);
    }
    if(scale <= 0){
        Usage(argv[0]);
    }
    trigger = scale * (1000000 / QUANTA);
    if(rate < 0) rate = 0;

    // Initialise the LEDs
    if (!led_init()) exit(-1);

    // Initialise current LED and current amount
    curled = 0;
    amt = 0;

    for (i = 0; i < ((unsigned int) dispsecs * 1000000) / QUANTA; i++){
        onfor(curled, 1, 0, 0, QUANTA);
        amt += rate;
        if(amt > trigger){
            ++curled;
	    if(curled >= LEDS) curled = 0;
            amt -= trigger;
        }
    }

    return 0;
}

void onfor(int led, int r, int g, int b, useconds_t usecs)
{
    led_on(led, r, g, b);
    usleep(usecs);
    led_off(led);
}


