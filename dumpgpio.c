#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"

int main(int argc, char **argv)
{
  int i;
  int mode;

  if(setup_gpio() != 0) exit(-1);

  for(i = 0 ; i < 54 ; i++){
    printf("GPIO %2d: ", i);
    mode = GPIO_MODE(i);
    printf("mode %d, ", mode);
    switch(mode){
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
    printf("\n");
  }
  printf("GPSET0: %08x\n", *(gpio+7));
  printf("GPSET1: %08x\n", *(gpio+8));
  printf("GPCLR0: %08x\n", *(gpio+10));
  printf("GPCLR1: %08x\n", *(gpio+11));
  printf("GPLEV0: %08x\n", *(gpio+13));
  printf("GPLEV1: %08x\n", *(gpio+14));


  return 0;
}

