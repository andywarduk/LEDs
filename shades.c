#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h"
#include "leds.h"

#define QUANTA 500

unsigned int patternbits = 12;
unsigned int bitpatterns[]={
  0x000, // 0000 0000 0000, 0 bits
  0x020, // 0000 0010 0000, 1 bit
  0x249, // 0010 0100 1001, 4 bits
  0xfff  // 1111 1111 1111, 12 bits
};

typedef struct {
  unsigned int r;
  unsigned int g;
  unsigned int b;
} leddef;

unsigned int phase = 0;

void onfor(int led, int r, int g, int b, useconds_t usecs);


int main(int argc, char **argv)
{
  unsigned int stage;
  unsigned int intensity;
  unsigned int i;
  unsigned int j;
  unsigned int r;
  unsigned int g;
  unsigned int b;
  leddef ledstate[5];

  if(!led_init()) exit(-1);

  stage = 1;
  while(1){
    // Set up led state
    for(i = 0 ; i < 5 ; i++){
      intensity = (i == 2 ? 1 : (i == 0 || i == 4) ? 3 : 2);
      ledstate[i].r = (stage & 1 ? intensity : 0);
      ledstate[i].g = (stage & 2 ? intensity : 0);
      ledstate[i].b = (stage & 4 ? intensity : 0);
    }

    // Display for one second
    for(j = 0 ; j < (1000000 / (QUANTA * 5 * patternbits)) ; j++){
      // For each phase
      for(phase=0; phase < patternbits; phase++){
        // For each led
        for(i = 0 ; i < 5 ; i++){
          r = (bitpatterns[ledstate[i].r] & (1 << phase) ? 1 : 0);
          g = (bitpatterns[ledstate[i].g] & (1 << phase) ? 1 : 0);
          b = (bitpatterns[ledstate[i].b] & (1 << phase) ? 1 : 0);
          onfor(i, r, g, b, QUANTA);
        }
      }
    }

    ++stage;
    if(stage == 8) stage = 1;
  }

  return 0;
}

void onfor(int led, int r, int g, int b, useconds_t usecs)
{
  led_on(led, r, g, b);
  usleep(usecs);
  led_off(led);
}


