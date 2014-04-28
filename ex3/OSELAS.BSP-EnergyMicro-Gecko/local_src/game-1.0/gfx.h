#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdio.h>

#include "globals.h"

#define BLACK 0x0000
#define WHITE 0xffff
#define BLUE 0x134f
#define RED 0xe063
#define GREEN 0x1fa3
#define YELLOW 0xefc0
#define CYAN 0x06ee
#define GRAY 0xc618
#define DARK_DARK_GRAY 0x5a73
#define DARK_GRAY 0x1101

#define BACKGROUND_COLOR DARK_DARK_GRAY
#define DIGIT_COLOR BLACK
#define FB_DRAW 0x4680

#define BLOCK_SIZE 15

static uint16_t colors[7] = {
    DARK_GRAY, WHITE, BLUE, RED, GREEN, YELLOW, CYAN
};

int fbfd;
uint16_t* fbp;
int screensize_pixels;
int screensize_bytes;
int blocks_per_width;
int blocks_per_height;

struct fb_var_screeninfo vinfo;
struct fb_copyarea grid;


int init_fb();
int cleanup_fb();
void refresh_fb(int x_min, int y_min, int x_max, int y_max);
void draw_board(int *board);
void draw_board_block(int pos, int val);
void draw_block(int x, int y, int val);
