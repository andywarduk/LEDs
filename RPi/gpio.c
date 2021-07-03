#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
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

static char *get_cpuinfo_revision();
static int get_gpio_revision(void);

unsigned bcm_host_get_peripheral_address();
unsigned bcm_host_get_peripheral_size();

int gpio_setup()
{
    int mem_fd;
    void *gpio_map = NULL;
    int rpi_revision = 0;
    unsigned peripheral_base;
    unsigned peripheral_len;
    unsigned gpio_base;

    // Check we're on a raspberry pi and get the revision
    rpi_revision = get_gpio_revision();

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

    // Get GPIO base address and length
    peripheral_base = bcm_host_get_peripheral_address();
    peripheral_len = bcm_host_get_peripheral_size();

#ifdef DEBUG
    printf("Peripheral base 0x%x, length 0x%x\n", peripheral_base, peripheral_len);
#endif

    gpio_base = peripheral_base + 0x200000;

#ifdef DEBUG
    printf("GPIO base 0x%x\n", gpio_base);
#endif

    // Open /dev/mem
    if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
        perror("Unable to open /dev/mem");
        return 0;
    }

    // mmap GPIO
    gpio_map = mmap(
                    NULL, // Any adddress in our space will do
                    4 * 1024, // Map length
                    PROT_READ | PROT_WRITE, // Enable reading & writting to mapped memory
                    MAP_SHARED, // Shared with other processes
                    mem_fd, // File to map
                    gpio_base // Offset to GPIO peripheral
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

static char *get_cpuinfo_revision()
{
    FILE *fp;
    char buffer[1024];
    char hardware[1024];
    char revision[1024];
    char *retrev;

    int rpi_found = 0;

    // Initialise revision string
    revision[0] = '\x0';

    // Open cpuinfo
    if ((fp = fopen("/proc/cpuinfo", "r")) == NULL) return NULL;

    // Read cuinfo line by line
    while (!feof(fp)) {
        fgets(buffer, sizeof(buffer), fp);
       
        // Look for hardware string
        sscanf(buffer, "Hardware  : %s", hardware);

        // If hardware begins with "BCM", assume raspberry PI found
        if (strncmp(hardware, "BCM", 3) == 0) rpi_found = 1;
       
        // Extract revision
        sscanf(buffer, "Revision  : %s", revision);
    }

    fclose(fp);

    // If BCM not found in hardware return NULL
    if (!rpi_found) return NULL;

    // If revision not found return NULL
    if (strlen(revision) == 0) return NULL;

    // Copy local revision and return it
    retrev = malloc(strlen(revision) + 1);
    strcpy(retrev, revision);

    return retrev;
}

static int get_gpio_revision(void)
{
    int rev = 0;
    char *revision;

    revision = get_cpuinfo_revision();

    if (revision != NULL) {
        if ((strcmp(revision, "0002") == 0) ||
            (strcmp(revision, "1000002") == 0 ) ||
            (strcmp(revision, "0003") == 0) ||
            (strcmp(revision, "1000003") == 0 ))
            rev = 1;
        else // assume rev 2+
            rev = 2;

        free(revision);
    }

    return rev;
}

static unsigned get_dt_ranges(const char *filename, unsigned offset)
{
    unsigned address = ~0;
    FILE *fp = fopen(filename, "rb");

    if (fp) {
        unsigned char buf[4];

        fseek(fp, offset, SEEK_SET);

        if (fread(buf, 1, sizeof buf, fp) == sizeof buf)
            address = buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0;

        fclose(fp);
    }

    return address;
}

unsigned bcm_host_get_peripheral_address()
{
   unsigned address = get_dt_ranges("/proc/device-tree/soc/ranges", 4);
   if (address == 0)
      address = get_dt_ranges("/proc/device-tree/soc/ranges", 8);
   return address == (unsigned) ~0 ? 0x20000000 : address;
}

unsigned bcm_host_get_peripheral_size()
{
   unsigned address = get_dt_ranges("/proc/device-tree/soc/ranges", 4);
   address = get_dt_ranges("/proc/device-tree/soc/ranges", (address == 0) ? 12 : 8);
   return address == (unsigned) ~0 ? 0x01000000 : address;
}
