
typedef struct
{
  int r;
  int g;
  int b;
} LEDDef;

class LEDs
{
public:
  // Pins
  static const int led1 = 3;
  static const int led2 = 4;
  static const int led3 = 5;
  static const int led4 = 6;
  static const int led5 = 7;

  static const int red = 8;
  static const int green = 9;
  static const int blue = 10;

  // Pin array
  static const int noLeds = 5;
  static const int ledPins[noLeds];

  LEDs();
  ~LEDs();

  void resetLeds();
  LEDDef *getLed(int led);
  void setLed(int led, int r, int g, int b);
  void dispLeds(int count, int delay);
  void shuffleDown();

private:
  LEDDef LEDState[noLeds];
  LEDDef *LEDOrder[noLeds];

  void dispLed(int led, int r, int g, int b, int delay);
};

