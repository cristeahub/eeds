#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void play_tone();

void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
    *TIMER1_IFC = 1;
    play_tone();
}

void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
    *GPIO_IFC = 0xff;
    *GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
    
    stopTimer();
    disableDAC();
}

void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    *GPIO_IFC = 0xff;
    *GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);

    startTimer();
    setupDAC();
}
