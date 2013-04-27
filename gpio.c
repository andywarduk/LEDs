#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "gpio.h"

#define BLOCK_SIZE (4*1024)

void *gpio_map;
volatile unsigned *gpio;

int setup_gpio()
{
    int mem_fd;

    /* open /dev/mem */
    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
        perror("Unable to open /dev/mem");
        return 0;
    }

    /* mmap GPIO */
    gpio_map = mmap(
                    NULL, // Any adddress in our space will do
                    BLOCK_SIZE, // Map length
                    PROT_READ | PROT_WRITE, // Enable reading & writting to mapped memory
                    MAP_SHARED, // Shared with other processes
                    mem_fd, // File to map
                    GPIO_BASE // Offset to GPIO peripheral
                    );

    if (gpio_map == MAP_FAILED) {
        perror("mmap error");
        return 0;
    }

    close(mem_fd); // No need to keep mem_fd open after mmap

    // Always use volatile pointer!
    gpio = (volatile unsigned *) gpio_map;

    return 1;
}

