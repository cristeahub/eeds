#include <stdint.h>
#include <stdlib.h>
#include "efm32gg.h"
#include "sound/blip.c"
#include "song.c"

void stopTimer();
extern int mode;

int i=0;

void play_tone() {
    if (i == 22050) {
        i = 0;
        stopTimer();
        mode = -1;
    } else {
        int sample = sound_sample(current_sound, i);
        *DAC0_CH0DATA = sample;
        *DAC0_CH1DATA = sample;
        i++;
    }
}

void play_song() {
    if (i == 353168) {
        i = 0;
        stopTimer();
        mode = -1;
    } else {
        *DAC0_CH0DATA = channel0[i];
        *DAC0_CH1DATA = channel1[i];
        i++;
    }
}
