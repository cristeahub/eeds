#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "sine.c"
#include "generate_wav.c"

#define S_RATE (44100)
#define BUF_SIZE (S_RATE*2)

int buffer[BUF_SIZE];

uint8_t int_sin(int n) {
    n = n % 256;
    return sine_wave[n];
}

int main() {
    int volume = 125;
    int freq_hz = 440;
    float phase = 0;

    /* ADSR
     *
     * Tweak the numbers:
     * a, d and r should be set in number of buffer frames
     * s is between 0 and 256, and represents the sustain level (volume)
     */
    int a = 1;
    int d = 10000;
    int s = 2;
    int r = 2000;

    int envelope[BUF_SIZE];
    for (int i=0; i < BUF_SIZE; i++) {
        if (i <= a) {
            envelope[i] = 256 * i / a;
        } else if (i <= (a+d)) {
            envelope[i] = 256 - (256-s)*(i-a)/d;
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
        buffer[i] = (int) (amplitude * int_sin(phase));
    }

    write_wav("test.wav", BUF_SIZE, buffer, S_RATE);

    return 0;
}
