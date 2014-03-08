#include <stdint.h>
#include <stdlib.h>
#include "efm32gg.h"
#include "sound/blip.c"

uint8_t tone[14] = {127, 182, 226, 251, 252, 227, 184, 129, 74, 29, 3, 2, 25, 69};
int i=0;

void play_tone() {
    if (i == 22050) {
        i = -1;
    }
    if (i >= 0) {
        int sample = sound_sample(current_sound, i);
        *DAC0_CH0DATA = sample;
        *DAC0_CH1DATA = sample;
        i++;
    }
}
