#include <stdint.h>
#include <stdbool.h>

#include "proto.h"
#include "efm32gg.h"
#define   SAMPLE_PERIOD   317

void setupTimer()
{
    *CMU_HFPERCLKEN0 |= CMU2_HFPERCLKEN0_TIMER1;
    *TIMER1_TOP = SAMPLE_PERIOD;
    *TIMER1_IEN = 1;
    *TIMER1_IFC = 1;
}

void disableTimer() {
    *TIMER1_CMD = 0;
    *CMU_HFPERCLKEN0 &= ~CMU2_HFPERCLKEN0_TIMER1;
}

void stopTimer() {
    *TIMER1_CMD = 0;
}

void startTimer() {
    *TIMER1_CMD = 1;
}
