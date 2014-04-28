#include "gfx.h"
#include "digit.h"

void draw_number(int number) {
    draw_number_with_color(number, DIGIT_COLOR);
}

void reset_number() {
    draw_number_with_color(8888, BACKGROUND_COLOR);
}

void draw_number_with_color(int number, uint16_t color) {
   int digit_x_anchor = 300 - DIGIT_WIDTH*DIGIT_BLOCK_SIZE;

   if(number == 0) {
        draw_digit(digit_x_anchor, DIGIT_Y_ANCHOR, digits[0], color);
   } else {
       while(number > 0) {
            int digit = number % 10;
            draw_digit(digit_x_anchor, DIGIT_Y_ANCHOR, digits[digit], color);

            number /= 10;
            digit_x_anchor -= DIGIT_WIDTH*DIGIT_BLOCK_SIZE;
            digit_x_anchor -= DIGIT_BLOCK_SIZE;
       }
   }
}

void draw_digit(int x, int y, int digit, uint16_t color) {
    if(x < 0) x = 0;
    if(y < 0) y = 0;
    if(digit < 9) digit = 0;

    int screen_index_x = 0;
    int screen_index_y = 0;
    int screen_index = 0;

    if(digit & 0b1000000) {
        // Line number 1. Straight line across. One dark block on either side
        for(int j = 0; j < DIGIT_BLOCK_SIZE; j++) {
            for(int i = 0; i < DIGIT_WIDTH * DIGIT_BLOCK_SIZE; i++) {
                if(i >= DIGIT_BLOCK_SIZE && i < DIGIT_WIDTH*DIGIT_BLOCK_SIZE - DIGIT_BLOCK_SIZE) {
                    screen_index_x = i + x;
                    screen_index_y = j + y;

                    screen_index = screen_index_y * vinfo.xres + screen_index_x;
                    fbp[screen_index] = color;
                }
            }
        }
    }

    if(digit & 0b0100000) {
        // Line number 2. Straight line down 2 blocks.
        for(int j = 0; j < DIGIT_BLOCK_SIZE * DIGIT_SIDE_HEIGHT; j++) {
            for(int i = 0; i < DIGIT_BLOCK_SIZE; i++) {
                screen_index_x = i + x;
                screen_index_y = j + y + DIGIT_BLOCK_SIZE;

                screen_index = screen_index_y * vinfo.xres + screen_index_x;
                fbp[screen_index] = color;
            }
        }
    }

    if(digit & 0b0010000) {
        // Line number 3. Straight line down 2 blocks.
        for(int j = 0; j < DIGIT_BLOCK_SIZE * DIGIT_SIDE_HEIGHT; j++) {
            for(int i = 0; i < DIGIT_BLOCK_SIZE; i++) {
                screen_index_x = i + x + DIGIT_BLOCK_SIZE*DIGIT_WIDTH - DIGIT_BLOCK_SIZE;
                screen_index_y = j + y + DIGIT_BLOCK_SIZE;

                screen_index = screen_index_y * vinfo.xres + screen_index_x;
                fbp[screen_index] = color;
            }
        }
    }

    if(digit & 0b0001000) {
        // Line number 4. Straight line across. One dark block on either side
        for(int j = 0; j < DIGIT_BLOCK_SIZE; j++) {
            for(int i = 0; i < DIGIT_WIDTH * DIGIT_BLOCK_SIZE; i++) {
                if(i >= DIGIT_BLOCK_SIZE && i < DIGIT_WIDTH*DIGIT_BLOCK_SIZE - DIGIT_BLOCK_SIZE) {
                    screen_index_x = i + x;
                    screen_index_y = j + y + 3 * DIGIT_BLOCK_SIZE;

                    screen_index = screen_index_y * vinfo.xres + screen_index_x;
                    fbp[screen_index] = color;
                }
            }
        }
    }

    if(digit & 0b0000100) {
        // Line number 5. Straight line down 2 blocks.
        for(int j = 0; j < DIGIT_BLOCK_SIZE * DIGIT_SIDE_HEIGHT; j++) {
            for(int i = 0; i < DIGIT_BLOCK_SIZE; i++) {
                screen_index_x = i + x;
                screen_index_y = j + y + 4 * DIGIT_BLOCK_SIZE;

                screen_index = screen_index_y * vinfo.xres + screen_index_x;
                fbp[screen_index] = color;
            }
        }
    }

    if(digit & 0b0000010) {
        // Line number 6. Straight line down 2 blocks.
        for(int j = 0; j < DIGIT_BLOCK_SIZE * DIGIT_SIDE_HEIGHT; j++) {
            for(int i = 0; i < DIGIT_BLOCK_SIZE; i++) {
                screen_index_x = i + x + DIGIT_BLOCK_SIZE*DIGIT_WIDTH - DIGIT_BLOCK_SIZE;
                screen_index_y = j + y + 4 * DIGIT_BLOCK_SIZE;

                screen_index = screen_index_y * vinfo.xres + screen_index_x;
                fbp[screen_index] = color;
            }
        }
    }

    if(digit & 0b0000001) {
        // Line number 7. Straight line across. One dark block on either side
        for(int j = 0; j < DIGIT_BLOCK_SIZE; j++) {
            for(int i = 0; i < DIGIT_WIDTH * DIGIT_BLOCK_SIZE; i++) {
                if(i >= DIGIT_BLOCK_SIZE && i < DIGIT_WIDTH*DIGIT_BLOCK_SIZE - DIGIT_BLOCK_SIZE) {
                    screen_index_x = i + x;
                    screen_index_y = j + y + 6 * DIGIT_BLOCK_SIZE;

                    screen_index = screen_index_y * vinfo.xres + screen_index_x;
                    fbp[screen_index] = color;
                }
            }
        }
    }
}
