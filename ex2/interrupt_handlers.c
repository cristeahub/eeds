#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

int lights = 0xffff;
/* TIMER1 interrupt handler */
void __attribute__ ((interrupt)) TIMER1_IRQHandler() 
{  
  /*
    TODO feed new samples to the DAC
    remember to clear the pending interrupt by writing 1 to TIMER1_IFC
  */  
    *TIMER1_IFC = 1;

    int i = 0xffffffff;
    while(i-->0);

    lights = ((lights & 0xff00) << 1 | (lights & 0x00ff) >> 1) & 0xff00;

    *GPIO_PA_DOUT = lights;
}

/* GPIO even pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_EVEN_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
    *GPIO_IFC = 0xff;
    *GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
}

/* GPIO odd pin interrupt handler */
void __attribute__ ((interrupt)) GPIO_ODD_IRQHandler() 
{
    /* TODO handle button pressed event, remember to clear pending interrupt */
    *GPIO_IFC = 0xff;
    *GPIO_PA_DOUT = (*GPIO_PC_DIN << 8);
}
