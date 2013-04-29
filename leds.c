#include <stdio.h>
#include <unistd.h>
#ifdef _POSIX_PRIORITY_SCHEDULING
#include <sched.h>
#endif
#include <string.h>
#include "leds.h"
#include "gpio.h"

int redpin;
int greenpin;
int bluepin;
int ledpins[5];

static void setRT();
static int get_pin(int hdr_pin, char *desc);
static void setup_pin(int gpio_num);

int led_init()
{
    unsigned int i;

    // Set up gpi pointer for direct register access
    if (!gpio_setup()) return 0;

    // Setup pin maps
    redpin = get_pin(11, "Red");
    greenpin = get_pin(13, "Green");
    bluepin = get_pin(15, "Blue");

    ledpins[0] = get_pin(7, "LED 5");
    ledpins[1] = get_pin(22, "LED 4");
    ledpins[2] = get_pin(18, "LED 3");
    ledpins[3] = get_pin(16, "LED 2");
    ledpins[4] = get_pin(12, "LED 1");

    // Set pins to output
    for (i = 0; i < sizeof (ledpins) / sizeof (int); i++) {
        setup_pin(ledpins[i]);
    }
    setup_pin(redpin);
    setup_pin(greenpin);
    setup_pin(bluepin);

    // Set led pins high
    for (i = 0; i < sizeof (ledpins) / sizeof (int); i++) {
        GPIO_SET(ledpins[i]);
    }

    // Set up real time scheduling for this process
    setRT();

    return 1;
}

static int get_pin(int hdr_num, char *desc)
{
    int gpio_num;

    gpio_num = gpio_pinmap[hdr_num];
#ifdef DEBUG
    printf("Header pin %d (%s) is GPIO %d\n", hdr_num, desc, gpio_num);
#endif

    return gpio_num;
}

static void setup_pin(int gpio_num)
{
    GPIO_INP(gpio_num); // Must use GPIO_INP before we can use GPIO_OUT
    GPIO_OUT(gpio_num);
}

static void setRT()
{
#ifdef _POSIX_PRIORITY_SCHEDULING
    int scheduler = SCHED_FIFO;
    struct sched_param schedparam;

    memset(&schedparam, 0, sizeof (struct sched_param));
    schedparam.sched_priority = sched_get_priority_max(scheduler);
    if (sched_setscheduler(0, scheduler, &schedparam) != 0) {
        perror("Unable to elevate to real time scheduling");
    }
#else
    printf("Real time scheduling not available\n");
#endif
}

void led_on(int led, int r, int g, int b)
{
    // Set red pin
    if (r)
        GPIO_SET(redpin);
    else
        GPIO_CLR(redpin);

    // Set green pin
    if (g)
        GPIO_SET(greenpin);
    else
        GPIO_CLR(greenpin);

    // Set blue pin
    if (b)
        GPIO_SET(bluepin);
    else
        GPIO_CLR(bluepin);

    // Set led pin low
    GPIO_CLR(ledpins[led]);
}

void led_off(int led)
{
    // Set led pin high
    GPIO_SET(ledpins[led]);
}


