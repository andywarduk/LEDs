// Sets LEDs to increasing intensity cycling through all colour combinations

#include "LEDs.h"


// Defines

// #define DEBUG 1

#define offsetof(st, m) __builtin_offsetof(st, m)
//#define offsetof(st, m) ((size_t)(&((st *)0)->m))


// Constants

// Timing
const int quanta = 10; // microseconds per led per %
const int noFrames = 4; // number of frames to hold pattern for

// Intensities
const int intensities[] = {
  0, 1, 4, 25, 100
};

const int noIntensity = (sizeof(intensities) / sizeof(int));

const int intensityValsLong[] = {
  0, 1, 1, 2, 2, 3, 3, 4, 4, 3, 3, 2, 2, 1, 1, 0
};

const int intensityValsShort[] = {
  0, 1, 2, 3, 4, 3, 2, 1
};

const int noIntensityValsLong = (sizeof(intensityValsLong) / sizeof(int));
const int noIntensityValsShort = (sizeof(intensityValsShort) / sizeof(int));


// Variables

LEDs leds;


// Functions

void setup()
{                
  int i;

#ifdef DEBUG
  Serial.begin(9600); 
  
  Serial.println("---=== Spectrum ===---");
  
  // Dump led pins
  Serial.print("ledPins["); Serial.print(LEDs::noLeds); Serial.print("] = {");
  for(i = 0; i < LEDs::noLeds; i++){
     if(i > 0) Serial.print(", ");
     Serial.print(LEDs::ledPins[i]);
  }
  Serial.println("}");
  
  // Dump RGB pins
  Serial.print("red = "); Serial.println(LEDs::red);
  Serial.print("green = "); Serial.println(LEDs::green);
  Serial.print("blue = "); Serial.println(LEDs::blue);
  
  // Print timing details
  Serial.print("LED quanta = "); Serial.print(quanta); Serial.println(" microseconds");
  Serial.print("LED PWM quanta = "); Serial.print(quanta * 100); Serial.println(" microseconds");
  Serial.print("LED frame = "); Serial.print(quanta * 100 * LEDs::noLeds); Serial.println(" microseconds");
  Serial.print("pattern frames = "); Serial.println(noFrames);
  Serial.print("LED pattern length = "); Serial.print(quanta * 100 * LEDs::noLeds * noFrames); Serial.println(" microseconds");

  // Dump intensities
  Serial.print("intensities["); Serial.print(noIntensity); Serial.print("] = {");
  for(i = 0; i < noIntensity; i++){
     if(i > 0) Serial.print(", ");
     Serial.print(intensities[i]);
     Serial.print("%");
  }
  Serial.println("}");

  Serial.print("intensityValsLong["); Serial.print(noIntensityValsLong); Serial.print("] = {");
  for(i = 0; i < noIntensityValsLong; i++){
     if(i > 0) Serial.print(", ");
     Serial.print(intensityValsLong[i]);
  }
  Serial.println("}");

  Serial.print("intensityValsShort["); Serial.print(noIntensityValsShort); Serial.print("] = {");
  for(i = 0; i < noIntensityValsShort; i++){
     if(i > 0) Serial.print(", ");
     Serial.print(intensityValsShort[i]);
  }
  Serial.println("}");
#endif
}


// Main loop

void loop()
{
  int stage;
  int p1;
  int p2;
  int p3;
  int l1;
  int l2;
  int l3;
  byte *stateBase;
  int i;
  int l;

  for(stage = 0; stage < 4; stage++){
#ifdef DEBUG
    Serial.print("stage="); Serial.print(stage); Serial.println();
#endif

    switch(stage){
    case 0:
      p1 = offsetof(LEDDef, b);
      p2 = offsetof(LEDDef, g);
      p3 = offsetof(LEDDef, r);
      break;
    case 1:
      p1 = offsetof(LEDDef, g);
      p2 = offsetof(LEDDef, r);
      p3 = offsetof(LEDDef, b);
      break;
    case 2:
      p1 = offsetof(LEDDef, r);
      p2 = offsetof(LEDDef, b);
      p3 = offsetof(LEDDef, g);
      break;
    }

#ifdef DEBUG
    Serial.print("p1="); Serial.print(p1); Serial.print(", ");
    Serial.print("p2="); Serial.print(p2); Serial.print(", ");
    Serial.print("p3="); Serial.print(p3); Serial.println();
#endif

    for(l1 = 0; l1 < noIntensityValsShort; l1++){
#ifdef DEBUG
      Serial.print("l1="); Serial.print(l1); Serial.println();
#endif
      for(l2 = 0; l2 < noIntensityValsShort; l2++){
        for(l3 = 0; l3 < noIntensityValsLong; l3++){
          // Set the LED
          stateBase = (byte *) leds.getLed(LEDs::noLeds - 1);
          *((int *)(stateBase + p1)) = intensities[intensityValsShort[l1]];
          *((int *)(stateBase + p2)) = intensities[intensityValsShort[l2]];
          *((int *)(stateBase + p3)) = intensities[intensityValsLong[l3]];

          // Display the LEDs
          leds.dispLeds(noFrames, quanta);

          // SHuffle the LEDs down
          leds.shuffleDown();          
        }
      }
    }
  }
}
