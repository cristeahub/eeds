#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#define _GNU_SOURCE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "main.h"
#include "pieces.h"
#include "gfx.h"
#include "digit.h"

#define __DEBUG__ 0

int *board;
FILE *gamepad;
unsigned char still_alive = true;
int score;

piece active_piece;

int main(int argc, char *argv[]) {
    int status;
    status = init();
    if(status == EXIT_FAILURE)
        goto error;
    loop();
    cleanup();

    return EXIT_SUCCESS;

error: ;
    return EXIT_FAILURE;
}

int init() {
    printf("Yes, this is game!");
    if(init_fb() == EXIT_FAILURE) {
        cleanup();
        return EXIT_FAILURE;
    }
    if(initialize_gamepad() == EXIT_FAILURE) {
        cleanup();
        return EXIT_FAILURE;
    }

    game_init();
    return EXIT_SUCCESS;
}

int initialize_gamepad() {
    gamepad = fopen("/dev/gamepad", "rb");
    if (!gamepad) {
        printf("Could not get access to gamepad. Is it loaded?\n");
        return EXIT_FAILURE;
    }
    long oflags;
    if (signal(SIGIO, &input_handler) == SIG_ERR) {
        printf("Could not register signal handler\n");
        return EXIT_FAILURE;
    }
    if (fcntl(fileno(gamepad), F_SETOWN, getpid()) == -1) {
        printf("Error setting owner\n");
        return EXIT_FAILURE;
    }
    oflags = fcntl(fileno(gamepad), F_GETFL);
    fcntl(fileno(gamepad), F_SETFL, oflags | FASYNC);

    return EXIT_SUCCESS;
}

void input_handler(int sig) {
    int input = fgetc(gamepad);
    parse_input(input);
}

void parse_input(int input) {
    int current_button = -1;
    input = ~input;
    for (int i=0; i < 8; i++) {
        if ((1 << i) == (input & (1 << i))) {
            // one-indexxxxd
            current_button = i + 1;
            break;
        }
    }

    printf("Current button: %d\n", current_button);

    printf("Still alive? %d\n", still_alive);
    if(!still_alive) {
        if (current_button == 6) {
            restart_game();
        }
        return;
    }

    switch (current_button) {
        case 1:
            left();
            update_board();
            break;
        case 3:
            right();
            update_board();
            break;
        case 4:
            if (active_piece.y < 0) {
                return;
            }
            place_piece(&board, active_piece);
            active_piece = spawn_piece();
            while (y_pos_for_piece(active_piece) < 0) {
                if (active_piece.x >= BOARD_WIDTH-PIECE_WIDTH) {
                    // No valid placements for piece
                    // Game over!
                    still_alive = false;
                    game_over();
                    break;
                }
                active_piece.x++;
            }
            update_board();
            break;

        case 5:
            rotate(3);
            update_board();
            break;
        case 6:
            rotate(0);
            update_board();
            break;
        case 7:
            rotate(1);
            update_board();
            break;
        case 8:
            rotate(2);
            update_board();
            break;
    }
}

void left() {
    int old_x = active_piece.x;
    do {
        if (active_piece.x <= 0) {
            active_piece.x = old_x;
            break;
        }
        active_piece.x--;
    } while (y_pos_for_piece(active_piece) < 0);
}

void right() {
    int old_x = active_piece.x;
    do {
        if (active_piece.x >= BOARD_WIDTH - PIECE_WIDTH) {
            active_piece.x = old_x;
            break;
        }
        active_piece.x++;
    } while (y_pos_for_piece(active_piece) < 0);
}

void rotate(int rotation) {
    int old_rotation = active_piece.rotation;
    active_piece.rotation = rotation;
    if (y_pos_for_piece(active_piece) < 0) {
        active_piece.rotation = old_rotation;
    }
}

void game_init() {
    board = initialize_board();
    initialize_pieces();
    draw_new_number(0);
}

void reset_board(int color) {
    for(int i = 0; i < BOARD_SIZE; i++) {
        board[i] = color;
    }
}

void game_over() {
    score = calculate_score(board);
    draw_new_number(score);
}

void restart_game() {
    still_alive = true;
    reset_board(0);
    draw_new_number(0);
    score = 0;
    update_board();
}

void draw_new_number(int number) {
    reset_number();
    draw_number(number);
    refresh_fb(DIGIT_GRID_X_MIN, DIGIT_GRID_Y_MIN, DIGIT_GRID_X_MAX, DIGIT_GRID_Y_MAX);
}

void update_board() {
    if (still_alive) {
        active_piece.y = y_pos_for_piece(active_piece);
        int *board_to_print = board_with_piece(board, active_piece);
        draw_board(board_to_print);
        free(board_to_print);
    } else {
        draw_board(board);
    }
    refresh_fb(BOARD_GRID_X_MIN, BOARD_GRID_Y_MIN, BOARD_GRID_X_MAX, BOARD_GRID_Y_MAX);

#if __DEBUG__
    print_board(board);
#endif /* __DEBUG__ */
}

void loop() {
    printf("Entering loop.\n");

    active_piece = spawn_piece();

    update_board();

    // goes to sleep
    while(1) pause();
}

void cleanup() {
    free(board);
}

int *initialize_board() {
    int *board = calloc(BOARD_SIZE, sizeof(int));
    return board;
}

int check_alive(int *board) {
    for(int i = 0; i < BOARD_WIDTH; ++i) {
        if(board[i] > 0) {
            return false;
        }
    }
    return true;
}

int calculate_score(int *board) {
    int score = 0;

    for(int i = 0; i < BOARD_SIZE; i++) {
        if(board[i] != 0) {
            int piece_color = board[i];
            int adjacent_blocks = 0;

            if(i+1 >= 0 && i+1 < BOARD_SIZE && board[i+1] == piece_color) {
                adjacent_blocks += board[i+1] ? 1 : 0;
            }
            if(i-1 >= 0 && i-1 < BOARD_SIZE && board[i+1] == piece_color) {
                adjacent_blocks += board[i-1] ? 1 : 0;
            }
            if(i+BOARD_WIDTH >= 0 && i+BOARD_WIDTH < BOARD_SIZE
                    && board[i+BOARD_WIDTH] == piece_color) {
                adjacent_blocks += board[i+BOARD_WIDTH] ? 1 : 0;
            }
            if(i-BOARD_WIDTH >= 0 && i-BOARD_WIDTH < BOARD_SIZE
                    && board[i-BOARD_WIDTH] == piece_color) {
                adjacent_blocks += board[i-BOARD_WIDTH] ? 1 : 0;
            }

            score += 1 + 2 * adjacent_blocks;
        }
    }

    reset_board(3);
    printf("Score: %d\n", score);
    return score;
}

int *board_with_piece(int *board, piece piece) {
    int *complete_board = malloc(sizeof(int) * BOARD_SIZE);
    int *piece_tile = get_piece_tile(piece);
    for (int i=0; i < BOARD_SIZE; ++i) {
        int board_x = i % BOARD_WIDTH;
        int board_y = i / BOARD_WIDTH;

        if ((board_x >= piece.x && board_x < piece.x + 4) &&
            (board_y >= piece.y && board_y < piece.y + 4)) {
            int piece_offset = board_x - piece.x + (board_y - piece.y) * PIECE_WIDTH;
            if (piece_tile[piece_offset]) {
                complete_board[i] = piece_tile[piece_offset] + 100;
            } else {
                if(board[i] > 100) {
                    board[i] -= 100;
                }
                complete_board[i] = board[i];
            }
        } else {
            if(board[i] > 100) {
                board[i] -= 100;
            }
            complete_board[i] = board[i];
        }
    }
    free(piece_tile);
    return complete_board;
}

void place_piece(int **board, piece piece) {
    int *new_board = board_with_piece(*board, piece);
    free(*board);
    *board = new_board;
}

void print_board(int *board) {
    int *print_board = board_with_piece(board, active_piece);
    for(int i = 0; i < BOARD_SIZE; ++i) {
        printf("%3d", print_board[i]);
        if((i+1) % BOARD_WIDTH == 0 && i != 0)
            printf("\n");
    }
    printf("\n");
}

int y_pos_for_piece(piece piece) {
    printf("Entered yposforpiece.\n");
    printf("Struct info: index: %d, x: %d, y: %d\n", piece.index, piece.x, piece.y);
    int y_pos = BOARD_HEIGHT-4;
    int *piece_tile = get_piece_tile(piece);
    while (1) {
        int clean = 1;
        for (int i=0; i < PIECE_SIZE; i++) {
            if (piece_tile[i]) {
                int x = i % PIECE_WIDTH;
                int y = i / PIECE_WIDTH;

                if (y_pos + y < 0) {
                    continue;
                }

                int board_offset = piece.x + x + (y_pos + y) * BOARD_WIDTH;
                if (board[board_offset]) {
                    clean = 0;
                    break;
                }
            }
        }
        if (clean) {
            break;
        } else {
            y_pos--;
            continue;
        }
    }
    printf("y_pos %d\n", y_pos);
    free(piece_tile);
    return y_pos;
}
