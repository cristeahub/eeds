#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "sine.c"
#include "generate_wav.c"

#define S_RATE (44100)
#define BUF_SIZE (S_RATE/2)

int buffer[BUF_SIZE];

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

int main() {
    int volume = 100;
    int freq_hz = 2000;
    float phase = 0;

    /* ADSR
     *
     * Tweak the numbers:
     * a, d and r should be set in number of buffer frames
     * s is between 0 and 256, and represents the sustain level (volume)
     */
    int a = 0;
    int d = 2050;
    int s = 128;
    int r = 20000;

    int envelope[BUF_SIZE];
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
        phase += freq_radians_per_sample;
        int amplitude = envelope[i] * volume / 256;
        buffer[i] = (int) (amplitude * int_square(phase));
    }

    write_wav("test.wav", BUF_SIZE, buffer, S_RATE);

    return 0;
}
