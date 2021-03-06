#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
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

unsigned int intensities[] = {
    0, 1, 1, 2, 2, 3, 3, 2, 2, 1, 1, 0
};
#define INTENSITIES (sizeof(intensities) / sizeof(unsigned int))

typedef struct
{
    unsigned int r;
    unsigned int g;
    unsigned int b;
} leddef;

void onfor(int led, int r, int g, int b, useconds_t usecs);


int main()
{
    int stage;
    int i;
    unsigned int j;

    unsigned int l1;
    unsigned int l2;
    unsigned int l3;

    void *statebase;
    unsigned int p1;
    unsigned int p2;
    unsigned int p3;

    unsigned int r;
    unsigned int g;
    unsigned int b;

    leddef *ledstate[LEDS];
    leddef ledstatearray[LEDS];
    leddef *tmpstate;

    unsigned int phase = 0;

    if (!led_init()) exit(-1);

    // Set up led state
    for (i = 0; i < LEDS; i++) {
        ledstatearray[i].r = 0;
        ledstatearray[i].g = 0;
        ledstatearray[i].b = 0;
        ledstate[i] = &ledstatearray[i];
    }

    stage = 1;
    while (1) {
        switch(stage){
        case 1:
            p1 = offsetof(leddef, b);
            p2 = offsetof(leddef, g);
            p3 = offsetof(leddef, r);
#ifdef DEBUG
            printf("Cycling Blue > Green > Red\n");
#endif
            break;
        case 2:
            p1 = offsetof(leddef, g);
            p2 = offsetof(leddef, r);
            p3 = offsetof(leddef, b);
#ifdef DEBUG
            printf("Cycling Green > Red > Blue\n");
#endif
            break;
        case 3:
            p1 = offsetof(leddef, r);
            p2 = offsetof(leddef, b);
            p3 = offsetof(leddef, g);
#ifdef DEBUG
            printf("Cycling Red > Blue > Green\n");
#endif
            break;
        }

        for (l1 = 0; l1 < INTENSITIES; l1++) {
            for (l2 = 0; l2 < INTENSITIES; l2++) {
                for (l3 = 0; l3 < INTENSITIES; l3++) {
                    statebase = (void *) ledstate[LEDS - 1];
                    *((unsigned int *)(statebase + p1)) = intensities[l1];
                    *((unsigned int *)(statebase + p2)) = intensities[l2];
                    *((unsigned int *)(statebase + p3)) = intensities[l3];

                    // Display for one tenth second
                    for (j = 0; j < (80000 / (QUANTA * LEDS * PATTERNBITS)); j++) {
                        // For each phase
                        for (phase = 0; phase < PATTERNBITS; phase++) {
                            // For each led
                            for (i = 0; i < LEDS; i++) {
                                r = (bitpatterns[ledstate[i]->r] & (1 << phase) ? 1 : 0);
                                g = (bitpatterns[ledstate[i]->g] & (1 << phase) ? 1 : 0);
                                b = (bitpatterns[ledstate[i]->b] & (1 << phase) ? 1 : 0);
                                onfor(i, r, g, b, QUANTA);
                            }
                        }
                    }

                    // Move
                    tmpstate = ledstate[0];
                    for (i = 0; i < LEDS - 1; i++) {
                        ledstate[i] = ledstate[i+1];
                    }
                    ledstate[LEDS - 1] = tmpstate;
                }
            }
        }

        ++stage;
        if (stage == 4) stage = 1;
    }

    return 0;
}

void onfor(int led, int r, int g, int b, useconds_t usecs)
{
    led_on(led, r, g, b);
    usleep(usecs);
    led_off(led);
}


