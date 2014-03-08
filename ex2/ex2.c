#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"
//#include "sound/blip.c"

#define   SAMPLE_PERIOD   317

void setupTimer(uint32_t period);
void stopTimer();
void startTimer();
void setupDAC();
void disableDAC();
void setupNVIC();
void setupGPIO();
void generate_blip();
void generate_laser();

int main(void) 
{  
  setupGPIO();
  setupDAC();
  setupTimer(SAMPLE_PERIOD);
  
  setupNVIC();

  generate_laser();

  __asm__("wfi");

  return 0;
}

void setupNVIC()
{
  *ISER0 = 0x1802;
}
