#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h"
#include "leds.h"

#define QUANTA 5000
#define LEDS 5

typedef struct
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
} leddef;

void onfor(int led, int r, int g, int b, useconds_t usecs);


void Usage(char *prog)
{
    printf("Usage: %s <percent> <dispsecs>\n", prog);
    exit(1);
}

int main(int argc, char **argv)
{
    int percentage;
    int dispsecs;

    unsigned int fullleds;
    unsigned int partial;

    unsigned int phase;
    unsigned int r;
    unsigned int g;
    unsigned int b;
    leddef ledstate[LEDS];

    unsigned int i, j, k;

    // Get args
    if(argc != 3){
        Usage(argv[0]);
    }
    percentage = atoi(argv[1]);
    dispsecs = atoi(argv[2]);
    if(dispsecs <= 0){
        Usage(argv[0]);
    }
    if(percentage < 0) percentage = 0;
    if(percentage > 100) percentage = 100;

    // Initialise the LEDs
    if (!led_init()) exit(-1);

    // Initialise the state
    for (i = 0; i < LEDS; i++) {
        ledstate[i].r = 0;
        ledstate[i].g = 0;
        ledstate[i].b = 0;
    }

    // Fully lit LEDs
    fullleds = percentage / (100 / LEDS);
    for (i = 0; i < fullleds; i++){
        ledstate[i].r = 2;
    }

    // Partially lit LED
    partial = percentage % (100 / LEDS);
    if (partial >= ((100 / LEDS) / 2)) ledstate[fullleds].r = 1;

    // Last LED green
    ledstate[LEDS - 1].g = ledstate[LEDS - 1].r;
    ledstate[LEDS - 1].r = 0;

    // Second from last LED amber
    ledstate[LEDS - 2].g = ledstate[LEDS - 2].r;

    if (percentage == 100){
        // Flash all LEDs if 100%
        for (i = 0; i < LEDS; i++){
            if(ledstate[i].r == 2) ledstate[i].r = 1;
            if(ledstate[i].g == 2) ledstate[i].g = 1;
            if(ledstate[i].b == 2) ledstate[i].b = 1;
        }
    }

    // Display for the required time period
    for (k = 0; k < (unsigned int) dispsecs; k++) {
	// 2 phases per second
        for(phase = 0; phase < 2; phase++){
            // Display for half a second
            for (j = 0; j < (500000 / (QUANTA * LEDS)); j++) {
                // For each led
                for (i = 0; i < LEDS; i++) {
                    r = (ledstate[i].r == 2 || (ledstate[i].r == 1 && phase == 0) ? 1 : 0);
                    g = (ledstate[i].g == 2 || (ledstate[i].g == 1 && phase == 0) ? 1 : 0);
                    b = (ledstate[i].b == 2 || (ledstate[i].b == 1 && phase == 0) ? 1 : 0);
                    onfor(i, r, g, b, QUANTA);
                }
            }
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


