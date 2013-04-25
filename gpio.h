#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define BLOCK_SIZE (4*1024)

extern volatile unsigned *gpio;
int setup_gpio();

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_ADDR(g) ((g) / 10)
#define GPIO_REG(g) (*(gpio + GPIO_ADDR(g)))
#define GPIO_SHIFT(g) (((g) % 10) * 3)
#define GPIO_MASK(g) (7 << GPIO_SHIFT(g))
#define GPIO_MODE(g) ((GPIO_REG(g) & GPIO_MASK(g)) >> GPIO_SHIFT(g))

#define GPIO_SET(g) (*(gpio + 7 + ((g) / 32)) = (1 << (g)))
#define GPIO_CLR(g) (*(gpio + 10 + ((g) / 32)) = (1 << (g)))

