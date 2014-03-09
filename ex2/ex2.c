#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

#define   SAMPLE_PERIOD   317

void setupTimer(uint32_t period);
void stopTimer();
void startTimer();
void setupDAC();
void disableDAC();
void setupNVIC();
void setupGPIO();

int main(void) 
{  
  setupGPIO();
  setupDAC();
  setupTimer(SAMPLE_PERIOD);
  
  setupNVIC();

  __asm__("wfi");

  return 0;
}

void setupNVIC()
{
  *ISER0 = 0x1802;
}
