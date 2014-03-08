#include <stdio.h>
#include "blip.c"
#include "generate_wav.c"

int main() {
    generate_blip();
    write_wav("test1.wav", BUF_SIZE, buffer, S_RATE);

    return 0;
}
