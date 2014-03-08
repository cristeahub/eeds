#include <stdint.h>
#include <stdbool.h>

#include "efm32gg.h"

void setupTimer(uint16_t period)
{
    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
    *TIMER1_TOP = period;
    *TIMER1_IEN = 1;
    *TIMER1_CMD = 0;
}

void stopTimer() {
    *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;
    *TIMER1_CMD = 0;
}

void startTimer() {
    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
    *TIMER1_CMD = 1;
}
