#include "gfx.h"
#include "globals.h"

int init_fb() {
    grid.dx = 0;
    grid.dy = 0;
    grid.width = 320;
    grid.height = 240;

    fbfd = open("/dev/fb0", O_RDWR);
    if(fbfd == -1) {
        printf("Unable to open framebuffer device.\n");
        return EXIT_FAILURE;
    }

    printf("The framebuffer was opened successufully.\n");

#if __FULLY__OPTIMIZED__
    screensize_pixels = 320 * 240; // width 320, height 240
    screensize_bytes = screensize_pixels * 16 / 8; // 16 bits per pixel, 8 bits per byte
    blocks_per_width = 320 / BLOCK_SIZE;
    blocks_per_height = 240 / BLOCK_SIZE;
#else
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        printf("Error reading variable information.\n");
        return EXIT_FAILURE;
    }
    printf("Successfully read info from the variable info\n");
    screensize_pixels = vinfo.xres * vinfo.yres;
    screensize_bytes = screensize_pixels * vinfo.bits_per_pixel / 8;

    blocks_per_width = vinfo.xres / BLOCK_SIZE;
    blocks_per_height = vinfo.yres / BLOCK_SIZE;
#endif

    printf("Blocks per height %d.\n", blocks_per_height);
    printf("Blocks her width %d.\n", blocks_per_width);

    fbp = (uint16_t*)mmap(NULL, screensize_bytes, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if((int)fbp == MAP_FAILED) {
        printf("Failed to memory map framebuffer.\n");
        return EXIT_FAILURE;
    }
    printf("Successfully memory mapped the frame buffer.\n");

    for(int i = 0; i < screensize_pixels; i++) {
        fbp[i] = BACKGROUND_COLOR;
    }

    // make two pixels on the left side and three on the right side black
    // to square out the screen
    for(int i = 0; i < vinfo.yres; i++) {
        fbp[i*vinfo.xres] = BLACK;
        fbp[i*vinfo.xres + 1] = BLACK;

        fbp[i*vinfo.xres + vinfo.xres-2] = BLACK;
        fbp[i*vinfo.xres + vinfo.xres-1] = BLACK;
        fbp[i*vinfo.xres + vinfo.xres] = BLACK;
    }

    ioctl(fbfd, FB_DRAW, &grid);

    printf("The screen should be blue and red now.\n");

    return EXIT_SUCCESS;
}

int cleanup_fb() {
    munmap(fbp, screensize_bytes);
    close(fbfd);
}

void refresh_fb(int x_min, int y_min, int x_max, int y_max) {
    grid.dx = x_min;
    grid.dy = y_min;
    grid.width = x_max - x_min;
    grid.height = y_max - y_min;
    printf("Refreshing framebuffer.\n");
    ioctl(fbfd, FB_DRAW, &grid);
}

void draw_board_block(int pos, int val) {
    unsigned char active_piece = false;
    if(val>100) {
        active_piece = true;
        val -= 100;
    }
    int x = 0;
    int y = 0;

    while(pos >= BOARD_WIDTH) {
        pos -= BOARD_WIDTH;
        y++;
    }

    x = pos;

    if(active_piece) {
        draw_bordered_block(x+1, y+1, val);
    } else {
        draw_block(x+1, y+1, val);
    }
}

void draw_board(int *board) {
    for(int i = 0; i < BOARD_SIZE; i++) {
        draw_board_block(i, board[i]);
    }
}

void draw_block(int x, int y, int color) {
    if(color > 6 || color < 0) {
        color = 0;
    }

    if(x < 0) x = 0;
    if(y < 0) y = 0;

    while(x >= blocks_per_width) {
        x -= blocks_per_width;
    }
    while(y >= blocks_per_height) {
        y -= blocks_per_height;
    }

    // there are five pixels too much, we black out two on the left side and
    // three on the other
    int screen_x = BLOCK_SIZE * x + 2;
    int screen_y = BLOCK_SIZE * y;
    int screen_index_x = 0;
    int screen_index_y = 0;
    int screen_index = 0;

    for(int j = 0; j < BLOCK_SIZE; j++) {
        for(int i = 0; i < BLOCK_SIZE; i++) {
            screen_index_x = i + screen_x;
            screen_index_y = j + screen_y;

            screen_index = screen_index_y * vinfo.xres + screen_index_x;

            fbp[screen_index] = colors[color];
        }
    }
}

void draw_bordered_block(int x, int y, int color) {
    if(color > 6 || color < 0) {
        color = 0;
    }

    if(x < 0) x = 0;
    if(y < 0) y = 0;

    while(x >= blocks_per_width) {
        x -= blocks_per_width;
    }
    while(y >= blocks_per_height) {
        y -= blocks_per_height;
    }

    // there are five pixels too much, we black out two on the left side and
    // three on the other
    int screen_x = BLOCK_SIZE * x + 2;
    int screen_y = BLOCK_SIZE * y;
    int screen_index_x = 0;
    int screen_index_y = 0;
    int screen_index = 0;

    for(int j = 0; j < BLOCK_SIZE; j++) {
        for(int i = 0; i < BLOCK_SIZE; i++) {
            screen_index_x = i + screen_x;
            screen_index_y = j + screen_y;

            screen_index = screen_index_y * vinfo.xres + screen_index_x;

            if(j <= 1 || i <= 1 || i >= BLOCK_SIZE - 1 || j >= BLOCK_SIZE - 1) {
                fbp[screen_index] = BLACK;
            } else {
                fbp[screen_index] = colors[color];
            }
        }
    }
}
