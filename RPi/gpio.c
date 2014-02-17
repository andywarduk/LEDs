#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include "gpio.h"

#define BLOCK_SIZE (4*1024)


volatile unsigned *gpio = NULL;
int *gpio_pinmap = NULL;


// Pin                        [0]    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26
// rev 1 map                       3v3  5v0  GP0  5V0  GP1  GND  GP4 GP14  GND GP15 GP17 GP18 GP21  GND GP22 GP23  3v3 GP24 GP10  GND  GP9 GP25 GP11  GP8  GND  GP7
static int gpio_pinmap_r1[] = {-1,  -1,  -1,   0,  -1,   1,  -1,   4,  14,  -1,  15,  17,  18,  21,  -1,  22,  23,  -1,  24,  10,  -1,   9,  25,  11,   8,  -1,   7};

// Pin                        [0]    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26
// rev 2 map                       3v3  5v0  GP2  5V0  GP3  GND  GP4 GP14  GND GP15 GP17 GP18 GP27  GND GP22 GP23  3v3 GP24 GP10  GND  GP9 GP25 GP11  GP8  GND  GP7
static int gpio_pinmap_r2[] = {-1,  -1,  -1,   2,  -1,   3,  -1,   4,  14,  -1,  15,  17,  18,  27,  -1,  22,  23,  -1,  24,  10,  -1,   9,  25,  11,   8,  -1,   7};

static char *get_cpuinfo_revision(char *revision);
static int get_rpi_revision(void);

int gpio_setup()
{
    int mem_fd;
    void *gpio_map = NULL;
    int rpi_revision = 0;

    // Check we're on a raspberry pi and get the revision
    rpi_revision = get_rpi_revision();
    switch (rpi_revision) {
    case 1:
        gpio_pinmap = gpio_pinmap_r1;
        break;
    case 2:
        gpio_pinmap = gpio_pinmap_r2;
        break;
    default:
	printf("Unable to get raspberry pi revision\n");
        return 0;
    }
#ifdef DEBUG
    printf("Running on Raspberry pi revision %d\n", rpi_revision);
#endif

    // open /dev/mem
    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
        perror("Unable to open /dev/mem");
        return 0;
    }

    // mmap GPIO
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
#ifdef DEBUG
    printf("GPIO mapped to address 0x%lx\n", (unsigned long) gpio_map);
#endif

    close(mem_fd); // No need to keep mem_fd open after mmap

    // Always use volatile pointer!
    gpio = (volatile unsigned *) gpio_map;

    return 1;
}

static char *get_cpuinfo_revision(char *revision)
{
    FILE *fp;
    char buffer[1024];
    char hardware[1024];
    int rpi_found = 0;

    if ((fp = fopen("/proc/cpuinfo", "r")) == NULL) return NULL;

    while (!feof(fp)) {
        fgets(buffer, sizeof(buffer), fp);
        sscanf(buffer, "Hardware  : %s", hardware);
        if (strcmp(hardware, "BCM2708") == 0) rpi_found = 1;
        sscanf(buffer, "Revision  : %s", revision);
    }
    fclose(fp);

    if (!rpi_found) revision = NULL;

    return revision;
}

static int get_rpi_revision(void)
{
    char revision[1024] = {'\0'};
   
    if (get_cpuinfo_revision(revision) == NULL) return 0;
      
    if ((strcmp(revision, "0002") == 0) ||
        (strcmp(revision, "1000002") == 0 ) ||
        (strcmp(revision, "0003") == 0) ||
        (strcmp(revision, "1000003") == 0 ))
        return 1;
    else // assume rev 2 (0004 0005 0006 1000004 1000005 1000006)
        return 2;
}

