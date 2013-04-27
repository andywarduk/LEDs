#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h"
#include "leds.h"

#define QUANTA 500
#define PATTERNBITS 12
#define LEDS 5

unsigned int bitpatterns[] = {
    0x000, // 0000 0000 0000, 0 bits
    0x020, // 0000 0010 0000, 1 bit
    0x249, // 0010 0100 1001, 4 bits
    0xfff  // 1111 1111 1111, 12 bits
};

typedef struct
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
} leddef;

unsigned int phase = 0;

void onfor(int led, int r, int g, int b, useconds_t usecs);


int main()
{
    int i;
    unsigned int j;

    unsigned int r;
    unsigned int g;
    unsigned int b;

    unsigned int ron;
    unsigned int gon;
    unsigned int bon;

    leddef *ledstate[LEDS];
    leddef ledstatearray[LEDS];
    leddef *tmpstate;

    if (!led_init()) exit(-1);

    // Set up led state
    for (i = 0; i < LEDS; i++) {
        ledstatearray[i].r = 0;
        ledstatearray[i].g = 0;
        ledstatearray[i].b = 0;
        ledstate[i] = &ledstatearray[i];
    }

    while (1) {
        for (r = 0; r < 6 ; r++) {
            for (g = 0; g < 6 ; g++) {
                for (b = 0; b < 6 ; b++) {
                    ledstate[LEDS - 1]->r = (r > 3 ? 6 - r : r);
                    ledstate[LEDS - 1]->g = (g > 3 ? 6 - g : g);
                    ledstate[LEDS - 1]->b = (b > 3 ? 6 - b : b);

                    // Display for one tenth second
                    for (j = 0; j < (90000 / (QUANTA * LEDS * PATTERNBITS)); j++) {
                        // For each phase
                        for (phase = 0; phase < PATTERNBITS; phase++) {
                            // For each led
                            for (i = 0; i < LEDS; i++) {
                                ron = (bitpatterns[ledstate[i]->r] & (1 << phase) ? 1 : 0);
                                gon = (bitpatterns[ledstate[i]->g] & (1 << phase) ? 1 : 0);
                                bon = (bitpatterns[ledstate[i]->b] & (1 << phase) ? 1 : 0);
                                onfor(i, ron, gon, bon, QUANTA);
                            }
                        }
                    }

                    // Move
                    tmpstate = ledstate[0];
                    for (i = 0 ; i < LEDS - 1 ; i++) {
                        ledstate[i] = ledstate[i+1];
                    }
                    ledstate[LEDS - 1] = tmpstate;
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


