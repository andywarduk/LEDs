#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "gpio.h"
#include "leds.h"

#define QUANTA 5000
#define LEDS 5

void onfor(int led, int r, int g, int b, useconds_t usecs);


void Usage(char *prog)
{
    printf("Usage: %s [-i LED] [-l] <rate> <scale> <dispsecs>\n", prog);
    exit(1);
}

int main(int argc, char **argv)
{
    int rate;
    int scale;
    int dispsecs;

    int curled = 0;
    unsigned int amt;
    unsigned int trigger;

    unsigned int i;

    unsigned int argcnt;
    char **nextarg;

    int r = 1;
    int g = 0;
    int b = 0;

    int leavelit = 0;

    argcnt = argc;
    nextarg = argv;

    ++nextarg;

    // Get args
    if(argcnt < 4){
        Usage(argv[0]);
    }

    do{
        if(strcmp(*nextarg, "-i") == 0){
	    ++nextarg;
	    curled = atoi(*nextarg);
            if(curled > LEDS) curled = 0;
            ++nextarg;
            argcnt -= 2;
        }
        else if(strcmp(*nextarg, "-l") == 0){
            leavelit = 1;
            ++nextarg;
            --argcnt;
        }
        else break;
    } while(1);

    if(argcnt != 4){
        Usage(argv[0]);
    }

    rate = atoi(*nextarg);
    ++nextarg;

    scale = atoi(*nextarg);
    ++nextarg;

    dispsecs = atoi(*nextarg);
    ++nextarg;

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
    amt = 0;

    for (i = 0; i < ((unsigned int) dispsecs * 1000000) / QUANTA; i++){
        onfor(curled, r, g, b, QUANTA);
        amt += rate;
        if(amt > trigger){
            ++curled;
	    if(curled >= LEDS) curled = 0;
            amt -= trigger;
        }
    }

    if(leavelit) led_on(curled, r, g, b);

    return curled;
}

void onfor(int led, int r, int g, int b, useconds_t usecs)
{
    led_on(led, r, g, b);
    usleep(usecs);
    led_off(led);
}


