#include <stdlib.h>
#include "time.h"
#include "pieces.h"

int **pieces;

void initialize_pieces() {

    int *piece1 = malloc(sizeof(int) * PIECE_SIZE);
    int p1[PIECE_SIZE] = {
        0, 0, 1, 0,
        1, 1, 1, 1,
        0, 0, 1, 0,
        0, 0, 1, 0
    };
    for(int i = 0; i < PIECE_SIZE; i++) {
        piece1[i] = p1[i];
    }

    int *piece2 = malloc(sizeof(int) * PIECE_SIZE);
    int p2[PIECE_SIZE] = {
        0, 1, 0, 0,
        1, 1, 0, 0,
        0, 1, 1, 0,
        0, 0, 1, 1
    };
    for(int i = 0; i < PIECE_SIZE; i++) {
        piece2[i] = p2[i];
    }

    int *piece3 = malloc(sizeof(int) * PIECE_SIZE);
    int p3[PIECE_SIZE] = {
        1, 0, 1, 0,
        1, 1, 1, 1,
        0, 1, 0, 0,
        0, 1, 1, 0
    };
    for(int i = 0; i < PIECE_SIZE; i++) {
        piece3[i] = p3[i];
    }

    int *piece4 = malloc(sizeof(int) * PIECE_SIZE);
    int p4[PIECE_SIZE] = {
        1, 0, 0, 0,
        1, 1, 0, 0,
        0, 1, 1, 1,
        0, 0, 1, 0
    };
    for(int i = 0; i < PIECE_SIZE; i++) {
        piece4[i] = p4[i];
    }

    pieces = malloc(sizeof(int*) * NUMBER_OF_PIECES);
    pieces[0] = piece1;
    pieces[1] = piece2;
    pieces[2] = piece3;
    pieces[3] = piece4;
}

int *get_piece_tile(piece piece) {
    int *piece_tile = get_piece_with_rotation(piece);
    for (int i=0; i < PIECE_SIZE; i++) {
        piece_tile[i] *= piece.color;
    }
    return piece_tile;
}

int *get_piece_with_rotation(piece piece) {
    int *new_piece = malloc(sizeof(int) * PIECE_SIZE);
    int rotation = piece.rotation % 4;
    switch (rotation) {
        case 0:
            for (int i=0; i < PIECE_SIZE; i++) {
                new_piece[i] = pieces[piece.index][i];
            }
            return new_piece;
            break;
        case 1:
            for (int i=0; i<4; i++) {
                for (int j=0; j<4; j++) {
                    new_piece[j*4 + 3-i] = pieces[piece.index][i*4 + j];
                }
            }
            break;
        case 2:
            for (int i=0; i<4; i++) {
                for (int j=0; j<4; j++) {
                    new_piece[3-j + 4*(3-i)] = pieces[piece.index][i*4 + j];
                }
            }
            break;
        case 3:
            for (int i=0; i<4; i++) {
                for (int j=0; j<4; j++) {
                    new_piece[4*(3-j) + i] = pieces[piece.index][i*4 + j];
                }
            }
            break;
    }
    while (1) {
        int clean_col = 1;
        for (int i=0; i < PIECE_SIZE; i+=4) {
            if (new_piece[i]) {
                clean_col = 0;
            }
        }
        if (clean_col == 0) {
            break;
        } else {
            for (int i=0; i<PIECE_SIZE; i+=4) {
                new_piece[i*4] = new_piece[i*4+1];
                for (int j=1; j<4; j++) {
                    new_piece[i*4+j-1] = new_piece[i*4+j];
                }
            }
        }
    }
    return new_piece;
}

piece spawn_piece() {
    srand(time(NULL));
    piece piece = {
        .index = rand() % 4,
        .x = 0,
        .rotation = 0,
        .color = rand() % 5 + 1
    };
    return piece;
}
