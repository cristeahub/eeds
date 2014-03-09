#include <stdint.h>
#include <stdlib.h>
#include "efm32gg.h"
#include "sound/blip.c"
#include "song.c"
#include "proto.h"


int i=0;

void setup_song() {
    setSleep(0b10);
    i = 0;
    setupDAC();
    setupTimer();
    startTimer();
}

void stop_song() {
    setSleep(0b110);
    i = 0;

    disableDAC();
    disableTimer();
}

void play_tone() {
    if (i == 22050) {
        stop_song();
    } else {
        int sample = sound_sample(current_sound, i);
        *DAC0_CH0DATA = sample;
        *DAC0_CH1DATA = sample;
        i++;
    }
}

void play_song() {
    if (i == 353168) {
        stop_song();
    } else {
        *DAC0_CH0DATA = channel0[i];
        *DAC0_CH1DATA = channel1[i];
        i++;
    }
}
