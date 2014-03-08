#include <stdint.h>
#include <stdlib.h>
#include "sine.c"

#define S_RATE (44100)
#define BUF_SIZE (S_RATE/2)

typedef struct Sound {
    int freq;
    int a;
    int d;
    int s;
    int r;
} Sound;

void generate_blip();
void generate_sound(int freq_hz, int a, int d, int s, int r);
int sound_sample(Sound sound, int i);

int int_sin(int n) {
    n = n % 256;
    return (sine_wave[n]*2)-256;
}
int int_square(int n) {
    n = n % 256;
    if (n < 128) {
        return -256;
    }
    return 256;
}
int int_sawtooth(int n) {
    return (n % 256) * 2 - 256;
}
int int_whitenoise() {
    return rand() % 512 - 256;
}

Sound current_sound;

void generate_blip() {
    int frequency = rand() % 1500 + 1500;
    int a = 0;
    int d = rand() % 3500;
    int s = rand() % 100 + 60;
    int r = rand() % 14000 + 4000;
    //generate_sound(frequency, a, d, s, r);
    current_sound = (Sound){frequency, a, d, s, r};
}

int sound_sample(Sound sound, int i) {
    int volume = 100;
    float phase = 0;

    int envelope;
    if (i < sound.a) {
        envelope = 255 * i / sound.a;
    } else if (i <= (sound.a+sound.d)) {
        envelope = 255 - (255-sound.s)*(i-sound.a)/sound.d;
    } else if (i < BUF_SIZE - sound.r) {
        envelope = sound.s;
    } else {
        envelope = sound.s*(BUF_SIZE-i)/sound.r;
    }

    float freq_radians_per_sample = sound.freq* 256.0 / S_RATE;

    if (i%50==0) {
        sound.freq -= 1;
        freq_radians_per_sample = sound.freq * 256.0 / S_RATE;
    }
    phase += freq_radians_per_sample;
    int amplitude = envelope * volume / 256;
    return (int) (amplitude * int_square(phase));
}


/*
void generate_sound(int freq_hz, int a, int d, int s, int r) {
    int volume = 100;
    float phase = 0;

    int *envelope = malloc(sizeof(int) * BUF_SIZE);
    for (int i=0; i < BUF_SIZE; i++) {
        if (i < a) {
            envelope[i] = 255 * i / a;
        } else if (i <= (a+d)) {
            envelope[i] = 255 - (255-s)*(i-a)/d;
        } else if (i < BUF_SIZE - r) {
            envelope[i] = s;
        } else {
            envelope[i] = s*(BUF_SIZE-i)/r;
        }
    }

    float freq_radians_per_sample = freq_hz * 256.0 / S_RATE;

    for (int i=0; i < BUF_SIZE; i++) {
        if (i%50==0) {
            freq_hz -= 1;
            freq_radians_per_sample = freq_hz * 256.0 / S_RATE;
        }
        phase += freq_radians_per_sample;
        int amplitude = envelope[i] * volume / 256;
        buffer[i] = (int) (amplitude * int_square(phase));
    }
    free(envelope);
}
*/
