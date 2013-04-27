#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"
#include "leds.h"

int main(int argc, char **argv)
{
  int i;

  if(!led_init()) exit(-1);

  // Switch LEDs off
  for(i = 0 ; i < 5 ; i++){
    led_off(i);
  }

  return 0;
}

