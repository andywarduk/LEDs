#include <Arduino.h>
#include "LEDs.h"

const int LEDs::ledPins[] = {
  led1, led2, led3, led4, led5
};

LEDs::LEDs()
{
  int i;

  // Set the LED pins as outputs, initialise the LED structs and pointers.
  for(i=0; i<noLeds; i++){
    pinMode(ledPins[i], OUTPUT);  
    digitalWrite(ledPins[i], HIGH);

    LEDOrder[i] = &LEDState[i];
  }

  // Reset the LED structs
  resetLeds();

  // Set the colour pins as outputs
  pinMode(red, OUTPUT);     
  pinMode(green, OUTPUT);     
  pinMode(blue, OUTPUT);       

  // Initialise the colour pins
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
}

LEDs::~LEDs()
{
  int i;

  for(i=0; i<noLeds; i++){
    digitalWrite(ledPins[i], HIGH);
  }  

  // Initialise the colour pins
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);
}

LEDDef *LEDs::getLed(int led)
{
  return LEDOrder[led];
}

void LEDs::setLed(int led, int r, int g, int b)
{
  LEDDef *actLed;

  actLed = LEDOrder[led];
  actLed->r = r;
  actLed->g = g;
  actLed->b = b;
}

void LEDs::dispLeds(int count, int delay)
{
  int i, l;
  LEDDef *actLed;

  for(i = 0; i < count; i++){
    for(l = 0; l < noLeds; l++){
      actLed = LEDOrder[l];
      dispLed(l, actLed->r, actLed->g, actLed->b, delay);
    }
  }
}

void LEDs::dispLed(int led, int r, int g, int b, int delay)
{
  int i;
  int rc;
  int gc;
  int bc;

  // Initialise the rgb pins
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);

  // Switch on the required led
  digitalWrite(ledPins[led], LOW);

  rc=0;
  gc=0;
  bc=0;  
  for(i=0; i<100; i++){
    rc+=r;
    if(rc>=100){
      digitalWrite(red, HIGH);
      rc-=100;
    }
    else{
      digitalWrite(red, LOW);
    }

    gc+=g;
    if(gc>=100){
      digitalWrite(green, HIGH);
      gc-=100;
    }
    else{
      digitalWrite(green, LOW);
    }

    bc+=b;
    if(bc>=100){
      digitalWrite(blue, HIGH);
      bc-=100;
    }
    else{
      digitalWrite(blue, LOW);
    }

    // Wait
    delayMicroseconds(delay);
  }

  // Switch off the led
  digitalWrite(ledPins[led], HIGH);  
}

void LEDs::resetLeds()
{
  int i;

  // Initialise the LED structs
  for(i = 0; i < noLeds; i++){
    LEDState[i].r = 0;
    LEDState[i].g = 0;
    LEDState[i].b = 0;
  }  
}

void LEDs::shuffleDown()
{
  LEDDef *tmpLed;
  int i;

  // Move
  tmpLed = LEDOrder[0];
  for (i = 0; i < LEDs::noLeds - 1; i++) {
    LEDOrder[i] = LEDOrder[i + 1];
  }
  LEDOrder[LEDs::noLeds - 1] = tmpLed;
}
