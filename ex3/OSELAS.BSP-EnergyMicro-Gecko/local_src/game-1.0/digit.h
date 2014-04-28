#ifndef __DIGIT__H__
#define __DIGIT__H__

#define DIGIT_0 0b1110111
#define DIGIT_1 0b0010010
#define DIGIT_2 0b1011101
#define DIGIT_3 0b1011011
#define DIGIT_4 0b0111010
#define DIGIT_5 0b1101011
#define DIGIT_6 0b1101111
#define DIGIT_7 0b1010010
#define DIGIT_8 0b1111111
#define DIGIT_9 0b1111010

static uint16_t digits[10] = {
    DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4,
    DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9
};

#define DIGIT_Y_ANCHOR 80

void draw_number(int number);
void reset_number();
void draw_number_with_color(int number, uint16_t color);
void draw_digit(int x, int y, int digit, uint16_t color);

#endif
