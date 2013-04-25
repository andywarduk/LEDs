// Header pin to GPIO mapping
#define PIN12 18
#define PIN16 23
#define PIN18 24
#define PIN22 25
#define PIN7 4
#define PIN11 17
#define PIN13_R1 21
#define PIN13_R2 27
#define PIN13 PIN13_R1 // Change to R2 on revision 2 boards
#define PIN15 22

// LED to pin mapping
#define LED1 PIN12
#define LED2 PIN16
#define LED3 PIN18
#define LED4 PIN22
#define LED5 PIN7

// Colour to pin mapping
#define RED PIN11
#define GREEN PIN13
#define BLUE PIN15

extern int pins[];
extern int ledpins[];

int led_init();
void led_on(int led, int r, int g, int b);
void led_off(int led);

