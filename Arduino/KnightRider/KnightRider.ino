// Sets LEDs to increasing intensity cycling through all colour combinations

#include "LEDs.h"


// Defines

// #define DEBUG 1


// Constants

// Timing
const int quanta = 10; // microseconds per led per %
const int noFrames = 5; // number of frames to hold pattern for

// Intensities
const int intensities[] = {
  0, 1, 4, 25, 100
};

const int noIntensity = (sizeof(intensities) / sizeof(int));

const int intensityVals[] = {
  1, 2, 3, 4
};

const int noIntensityVals = (sizeof(intensityVals) / sizeof(int));


// Variables

typedef struct
{
  int r;
  int g;
  int b;
} 
ledDef;

ledDef ledStruct[LEDs::noLeds];

LEDs ledsClass;


// Functions

void setup()
{                
  int i;

#ifdef DEBUG
  Serial.begin(9600); 

  Serial.println("---=== KnightRider ===---");

  // Dump led pins
  Serial.print("ledPins["); Serial.print(LEDs::noLeds); Serial.print("] = {");
  for(i=0; i<LEDs::noLeds; i++){
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
  Serial.print("intensities["); Serial.print(noIntensity);  Serial.print("] = {");
  for(i=0; i<noIntensity; i++){
    if(i > 0) Serial.print(", ");
    Serial.print(intensities[i]);
    Serial.print("%");
  }
  Serial.println("}");

  Serial.print("intensityVals["); Serial.print(noIntensityVals); Serial.print("] = {");
  for(i=0; i<noIntensityVals; i++){
    if(i > 0) Serial.print(", ");
    Serial.print(intensityVals[i]);
  }
  Serial.println("}");
#endif

  // initialise the LED structs and pointers.
  for(i=0; i<LEDs::noLeds; i++){
    ledStruct[i].r = 0;
    ledStruct[i].g = 0;
    ledStruct[i].b = 0;
  }
}


// Main loop

int position = 0;
int direction = 1;

void loop()
{
  int i,l;
  int intensity;

  // Set up led state
  for (i = 0; i < LEDs::noLeds; i++) {
    ledStruct[i].r = 0;
  }
  for (intensity = 3, i = position; intensity > 0 && i >= 0 && i < LEDs::noLeds; intensity--, i -= direction) {
    ledStruct[i].r = intensities[intensityVals[intensity]];
  }

  // For each frame
  for (i = 0; i < noFrames; i++) {
    // For each led
    for (l = 0; l < LEDs::noLeds; l++) {
      ledsClass.setLed(l, ledStruct[l].r, ledStruct[l].g, ledStruct[l].b, quanta);
    }
  }

  // Move the blob
  if (position == 0) direction = 1;
  if (position == LEDs::noLeds - 1) direction = -1;
  position += direction;
}

