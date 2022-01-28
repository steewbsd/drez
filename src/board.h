#ifndef BOARD_H
#define BOARD_H
#include "piece.h"

#define FLAG_PIN       0x1 /* block piece when it would discover a check */
#define FLAG_CHECK     0x2 /* block moves when king is in check (additional checking
							  should be done for moves preventing check) */
#define FLAG_FIRSTMOVE 0x4 /* for castles and pawn starts */

typedef struct {
  /* TODO: piece location hash table */
  /*            rank      file     */
  cell game[SIZE_STD][SIZE_STD];
} board;

board *init_board();
void free_board(board *game);
int move(position origin, position target, board *game);
#endif
