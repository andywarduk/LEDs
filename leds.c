#include "leds.h"
#include "gpio.h"

int pins[] = {LED1, LED2, LED3, LED4, LED5, RED, GREEN, BLUE};
int ledpins[] = {LED5, LED4, LED3, LED2, LED1};

int led_init()
{
  unsigned int i;

  // Set up gpi pointer for direct register access
  if(!setup_gpio()) return 0;

  // Set pins to output
  for (i = 0 ; i < sizeof(pins) / sizeof(int) ; i++){
    INP_GPIO(pins[i]); // must use INP_GPIO before we can use OUT_GPIO
    OUT_GPIO(pins[i]);
  }

  // Set led pins high
  for(i = 0 ; i < sizeof(ledpins) / sizeof(int) ; i++){
    GPIO_SET(ledpins[i]);
  }

  return 1;
}

void led_on(int led, int r, int g, int b)
{
  // Set red pin
  if(r)
    GPIO_SET(RED);
  else
    GPIO_CLR(RED);

  // Set green pin
  if(g)
    GPIO_SET(GREEN);
  else
    GPIO_CLR(GREEN);

  // Set blue pin
  if(b)
    GPIO_SET(BLUE);
  else
    GPIO_CLR(BLUE);

  // Set led pin low
  GPIO_CLR(ledpins[led]);
}

void led_off(int led)
{
  // Set led pin high
  GPIO_SET(ledpins[led]);  
}


