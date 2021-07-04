#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"

void dump_reg(int offset, char *name, unsigned bitmod) {
    unsigned value;

    value = *(gpio + offset);
    fprintf(stdout, "  GP%-7s : 0x%08x : ", name, value);

    for (int bit = 31; bit >= 0; bit--) {
        if (value & (1 << bit)) {
            putc('1', stdout);
        } else {
            putc('0', stdout);
        }

        if (bitmod && (bit % bitmod) == 0) {
            putc(' ', stdout);
        }
    }
    fputs("\n", stdout);
}

void dump_regn(int offset, int reg, char *name, unsigned bitmod) {
    char fullname[32];

    sprintf(fullname, "%s%d", name, reg);
    dump_reg(offset + reg, fullname, bitmod);
}

int find_pin(int gpio) {
    for (unsigned i = 1; i < gpio_pins; i++) {
        if (gpio_pinmap[i] == gpio) return i;
    }

    return -1;
}

int main()
{
    int i;
    int mode;

    if (!gpio_setup()) exit(-1);

    printf("Register dump:\n");

    for (i = 0; i < 6; i++) {
        dump_regn(0, i, "FSEL", 3);
    }

    for (i = 0; i < 2; i++) {
        dump_regn(13, i, "LEV", 4);
    }

    for (i = 0; i < 2; i++) {
        dump_regn(16, i, "EDS", 4);
    }

    for (i = 0; i < 2; i++) {
        dump_regn(19, i, "REN", 4);
    }

    for (i = 0; i < 2; i++) {
        dump_regn(22, i, "FEN", 4);
    }

    for (i = 0; i < 2; i++) {
        dump_regn(25, i, "HEN", 4);
    }

    for (i = 0; i < 2; i++) {
        dump_regn(28, i, "LEN", 4);
    }

    for (i = 0; i < 2; i++) {
        dump_regn(31, i, "AREN", 4);
    }

    for (i = 0; i < 2; i++) {
        dump_regn(34, i, "AFEN", 4);
    }

    dump_reg(37, "PUD", 4);
    for (i = 0; i < 2; i++) {
        dump_regn(38, i, "PUDCLK", 4);
    }

    for (i = 0; i < 54; i++) {
        printf("GPIO %2d: ", i);

        int pin = find_pin(i);

        if (pin >=0 ) {
            printf("pin %d, ", pin);
        }
        
        mode = GPIO_FSEL_MODE(i);
        printf("mode %d, ", mode);
        
        switch (mode) {
        case 0:
            printf("input");
            break;
        case 1:
            printf("output");
            break;
        case 4:
            printf("alt function 0");
            break;
        case 5:
            printf("alt function 1");
            break;
        case 6:
            printf("alt function 2");
            break;
        case 7:
            printf("alt function 3");
            break;
        case 3:
            printf("alt function 4");
            break;
        case 2:
            printf("alt function 5");
            break;
        }
        
        printf(", level %d ", GPIO_LEV(i));
        
        printf("\n");
    }
    
    return 0;
}

