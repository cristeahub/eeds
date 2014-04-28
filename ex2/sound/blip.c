#include <stdint.h>
#include "../rand.c"

#define S_RATE (44100)
#define BUF_SIZE (S_RATE/2)

extern int i;
typedef struct Sound {
    int freq;
    int a;
    int d;
    int s;
    int r;
    int slide;
    int (*wave)(int);
} Sound;

int sound_sample(Sound sound, int i);

int int_square(int n) {
    n = n % 256;
    if (n < 128) {
        return 0;
    }
    return 2;
}
int int_sawtooth(int n) {
    n = n % 256;
    if (n < 84) {
        return 0;
    }
    if (n < 168) {
        return 1;
    }
    return 2;
}
int int_whitenoise(int n) {
    return rand() % 3;
}

Sound current_sound;

void generate_blip() {
    int frequency = rand() % 1500 + 1500;
    int a = 0;
    int d = rand() % 3500;
    int s = rand() % 50 + 30;
    int r = rand() % 14000 + 4000;
    int slide = 0;
    current_sound = (Sound){frequency, a, d, s, r, slide, int_square};
    i = 0;
}

void generate_laser() {
    int frequency = rand() % 2000 + 1200;
    int a = 0;
    int d = rand() % 2000 + 1000;
    int s = rand() % 50;
    int r = rand() % 6000 + 4500;
    int slide = 2;
    int func_i = rand() % 2;
    int (*wave)(int) = (func_i) ? int_square : int_sawtooth;
    current_sound = (Sound){frequency, a, d, s, r, slide, wave};
    i = 0;
}

void generate_blurp() {
    int frequency = rand() % 1000 + 100;
    int a = rand() % 500;
    int d = rand() % 2000 + 1000;
    int s = rand() % 50 + 50;
    int r = rand() % 14000 + 2500;
    int slide = - ( rand() % 3 + 3);
    current_sound = (Sound){frequency, a, d, s, r, slide, int_sawtooth};
    i = 0;
}

void generate_alert() {
    int frequency = rand() % 80 + 175;
    int a = 0;
    int d = rand() % 1000 + 500;
    int s = rand() % 50 + 50;
    int r = rand() % 14000 + 2500;
    int slide = 0;
    current_sound = (Sound){frequency, a, d, s, r, slide, int_sawtooth};
    i = 0;
}

int sound_sample(Sound sound, int i) {

    int envelope;
    if (i < sound.a) {
        envelope = 127 * i / sound.a;
    } else if (i < (sound.a+sound.d)) {
        envelope = 127 - (127-sound.s)*(i-sound.a)/sound.d;
    } else if (i < BUF_SIZE - sound.r) {
        envelope = sound.s;
    } else {
        envelope = sound.s*(BUF_SIZE-i)/sound.r;
    }

    int slide = 0;
    if (sound.slide != 0) slide = i/sound.slide;
    int freq = sound.freq - slide;
    int freq_radians_per_sample = freq * 256 / S_RATE;

    int phase = (i+1) * freq_radians_per_sample;
    //float amplitude = envelope / 256.0;
    return (int) (envelope * sound.wave(phase));
}
