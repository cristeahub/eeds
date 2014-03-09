#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
#include "proto.h"

int main(void) 
{  
  setupGPIO();
  setupNVIC();
  setSleep(0b110);

  __asm__("wfi");

  return 0;
}

void setupNVIC()
{
    *ISER0 = 0x1802;
}

void setSleep(int arg) {
    *SCR = arg;
}
