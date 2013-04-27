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
    0xfff // 1111 1111 1111, 12 bits
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
    unsigned int position = 0;
    int direction = 1;
    unsigned int intensity;
    int i;
    unsigned int j;

    unsigned int r;
    unsigned int g;
    unsigned int b;
    leddef ledstate[LEDS];

    if (!led_init()) exit(-1);

    // Initialise the state
    for (i = 0; i < LEDS; i++) {
        ledstate[i].r = 0;
        ledstate[i].g = 0;
        ledstate[i].b = 0;
    }

    while (1) {
        // Set up led state
        for (i = 0; i < LEDS; i++) {
            ledstate[i].r = 0;
        }
        for (intensity = 3, i = position; intensity > 0 && i >= 0 && i < LEDS; intensity--, i -= direction) {
            ledstate[i].r = intensity;
        }

        // Display for one tenth second
        for (j = 0; j < (80000 / (QUANTA * LEDS * PATTERNBITS)); j++) {
            // For each phase
            for (phase = 0; phase < PATTERNBITS; phase++) {
                // For each led
                for (i = 0; i < LEDS; i++) {
                    r = (bitpatterns[ledstate[i].r] & (1 << phase) ? 1 : 0);
                    g = (bitpatterns[ledstate[i].g] & (1 << phase) ? 1 : 0);
                    b = (bitpatterns[ledstate[i].b] & (1 << phase) ? 1 : 0);
                    onfor(i, r, g, b, QUANTA);
                }
            }
        }

        // Move the blob
        if (position == 0) direction = 1;
        if (position == LEDS-1) direction = -1;
        position += direction;
    }

    return 0;
}

void onfor(int led, int r, int g, int b, useconds_t usecs)
{
    led_on(led, r, g, b);
    usleep(usecs);
    led_off(led);
}


