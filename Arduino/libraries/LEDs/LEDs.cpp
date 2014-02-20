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
  }  

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

void LEDs::setLed(int led, int r, int g, int b, int delay)
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

