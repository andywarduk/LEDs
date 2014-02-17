extern int ledpins[];
extern int redpin;
extern int greenpin;
extern int bluepin;

int led_init();
void led_on(int led, int r, int g, int b);
void led_off(int led);
