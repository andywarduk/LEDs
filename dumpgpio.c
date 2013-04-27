#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"

int main()
{
    int i;
    int mode;

    if (!setup_gpio()) exit(-1);

    printf("Register dump:\n");
    printf("  GPFSEL0: %08x\n", *(gpio + 0));
    printf("  GPFSEL1: %08x\n", *(gpio + 1));
    printf("  GPFSEL2: %08x\n", *(gpio + 2));
    printf("  GPFSEL3: %08x\n", *(gpio + 3));
    printf("  GPFSEL4: %08x\n", *(gpio + 4));
    printf("  GPFSEL5: %08x\n", *(gpio + 5));

    printf("  GPSET0: %08x\n", *(gpio + 7));
    printf("  GPSET1: %08x\n", *(gpio + 8));
    
    printf("  GPCLR0: %08x\n", *(gpio + 10));
    printf("  GPCLR1: %08x\n", *(gpio + 11));
    
    printf("  GPLEV0: %08x\n", *(gpio + 13));
    printf("  GPLEV1: %08x\n", *(gpio + 14));

    for (i = 0; i < 54; i++) {
        printf("GPIO %2d: ", i);
        
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

