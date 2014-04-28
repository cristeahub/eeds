#ifndef __GAME__MAIN__H__
#define __GAME__MAIN__H__

#include "globals.h"

typedef struct piece {
    int index;
    int x;
    int y;
    int rotation;
    int color;
} piece;

void input_handler(int sig);
int initialize_gamepad();
int *initialize_board();
void parse_input(int input);
void print_board(int *board);
int check_alive(int *board);
int calculate_score(int *board);
void game_init();
int init();
void loop();
void cleanup();
void draw_new_number(int number);
int y_pos_for_piece(piece);
int *board_with_piece(int *, piece);
void update_board();
void place_piece(int**, piece);
void game_over();
void restart_game();
void reset_board(int color);

void left();
void right();
void rotate(int);

#endif
