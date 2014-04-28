#ifndef __GAME__PIECES__H__
#define __GAME__PIECES__H__

#include "main.h"

#define NUMBER_OF_PIECES 4

void initialize_pieces();
int *get_piece_tile(piece);
int *get_piece_with_rotation(piece);
piece spawn_piece();

#endif
