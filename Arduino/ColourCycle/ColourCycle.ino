// Colour cycle colours & intensities

#include "LEDs.h"


// Defines

// #define DEBUG 1


// Constants

// Timing
const int quanta = 10; // microseconds per led per %
const int noFrames = 8; // number of frames to hold pattern for

// Intensities
const int intensities[] = {
  0, 1, 4, 25, 100
};

const int noIntensity = (sizeof(intensities) / sizeof(int));

const int intensityVals[] = {
  1, 2, 3, 4
};

const int noIntensityVals = (sizeof(intensityVals) / sizeof(int));


// Prototypes

void nextLed(int l);


// Variables

LEDs leds;

int colour = 0;
int coldir = 1;
int intensity = 0;
int intdir = 1;


// Functions

void setup()
{
  int i;

#ifdef DEBUG
  Serial.begin(9600); 
  
  Serial.println("---=== ColourCycle ===---");
  
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

  Serial.print("intensityVals["); Serial.print(noIntensityVals); Serial.print("] = {");
  for(i = 0; i < noIntensityVals; i++){
     if(i > 0) Serial.print(", ");
     Serial.print(intensityVals[i]);
  }
  Serial.println("}");
#endif

  for(i = 0; i < LEDs::noLeds; i++){
    nextLed(i);
  }
}


// Main loop

void loop()
{
  // Display the LEDs
  leds.dispLeds(noFrames, quanta);

  // Shuffle the LEDs
  leds.shuffleDown();

  // Set next
  nextLed(LEDs::noLeds - 1);
}


// Functions

void nextLed(int l)
{
  int r, g, b;

  r = (colour & 0x1 ? intensities[intensityVals[intensity]] : 0);
  g = (colour & 0x2 ? intensities[intensityVals[intensity]] : 0);
  b = (colour & 0x4 ? intensities[intensityVals[intensity]] : 0);

#ifdef DEBUG
  Serial.print("c=");
  Serial.print(colour);
  Serial.print(" i=");
  Serial.print(intensity);
  Serial.print(" l=");
  Serial.print(l);
  Serial.print(" r=");
  Serial.print(r);
  Serial.print(" g=");
  Serial.print(g);
  Serial.print(" b=");
  Serial.print(b);
  Serial.println();
#endif

  leds.setLed(l, r, g, b);

  if(coldir == 1){
    ++colour;
    if(colour == 7){
      coldir = -1;
    }
  }
  else{
    --colour;
    if(colour == 1){
      coldir = 1;

      // Change intensity
      intensity += intdir;
      if(intdir == 1 && intensity == noIntensityVals - 1) intdir = -1;
      else if(intdir == -1 && intensity == 0) intdir = 1;
    }
  }
}
