#include <stdint.h>
#include "efm32gg.h"

uint8_t tone[14] = {127, 182, 226, 251, 252, 227, 184, 129, 74, 29, 3, 2, 25, 69};
int i=0;

void play_tone() {
    if (i == 14) i=0;
    *DAC0_CH0DATA = tone[i];
    *DAC0_CH1DATA = tone[i];
    i++;
}
