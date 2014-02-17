extern volatile unsigned int *gpio;
extern int *gpio_pinmap;
int gpio_setup();


#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) // GPIO controller


// Function select register
#define GPIO_FSEL_OFFSET(g) ((g) / 10)
#define GPIO_FSEL_REG(g) (*(gpio + GPIO_FSEL_OFFSET(g)))
#define GPIO_FSEL_SHIFT(g) (((g) % 10) * 3)
#define GPIO_FSEL_MASK(g) (7 << GPIO_FSEL_SHIFT(g))
#define GPIO_FSEL_MODE(g) ((GPIO_FSEL_REG(g) & GPIO_FSEL_MASK(g)) >> GPIO_FSEL_SHIFT(g))

// Always use GPIO_INP(x) before using GPIO_OUT(x) or GPIO_ALT(x, y)
#define GPIO_INP(g) GPIO_FSEL_REG(g) &= ~(GPIO_FSEL_MASK(g))
#define GPIO_OUT(g) GPIO_FSEL_REG(g) |=  (1 << GPIO_FSEL_SHIFT(g))
#define GPIO_ALT(g, a) GPIO_FSEL_REG(g) |= (((a) <= 3 ? (a) + 4 : (a) == 4 ? 3 : 2 ) << GPIO_FSEL_SHIFT(g))


// Pin set registers
#define GPIO_SET_OFFSET(g) (7 + ((g) / 32))
#define GPIO_SET_REG(g) (*(gpio + GPIO_SET_OFFSET(g)))
#define GPIO_SET_SHIFT(g) ((g) % 32)
#define GPIO_SET(g) (GPIO_SET_REG(g) = (1 << GPIO_SET_SHIFT(g)))


// Pin clear registers
#define GPIO_CLR_OFFSET(g) (10 + ((g) / 32))
#define GPIO_CLR_REG(g) (*(gpio + GPIO_CLR_OFFSET(g)))
#define GPIO_CLR_SHIFT(g) ((g) % 32)
#define GPIO_CLR(g) (GPIO_CLR_REG(g) = (1 << GPIO_CLR_SHIFT(g)))


// Pin level registers
#define GPIO_LEV_OFFSET(g) (13 + ((g) / 32))
#define GPIO_LEV_REG(g) (*(gpio + GPIO_LEV_OFFSET(g)))
#define GPIO_LEV_SHIFT(g) ((g) % 32)
#define GPIO_LEV(g) ((GPIO_LEV_REG(g) & (1 << GPIO_LEV_SHIFT(g))) >> GPIO_LEV_SHIFT(g))
