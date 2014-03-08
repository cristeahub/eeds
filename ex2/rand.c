#include <stdint.h>

static unsigned int next = 1;

int rand_r(unsigned int *seed) {
    *seed = *seed * 1103515245 + 12345;
    return (*seed % ((unsigned int)RAND_MAX + 1));
}

int rand() {
    return (rand_r(&next));
}
