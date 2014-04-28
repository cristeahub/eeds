#include <stdint.h>
#include <stdbool.h>

#include "proto.h"
#include "efm32gg.h"

void setupDAC()
{
    *CMU_HFPERCLKEN0 |= (1 << 17);
    *DAC0_CTRL = 0x50014;
    *DAC0_CH0CTRL = 1;
    *DAC0_CH1CTRL = 1;
}

void disableDAC() {
    *DAC0_CTRL = 0;
    *DAC0_CH0CTRL = 0;
    *DAC0_CH1CTRL = 0;
    *CMU_HFPERCLKEN0 &= ~(1 << 17);
}
