#ifndef BOARD_H
#define BOARD_H
#include "piece.h"

typedef struct {
  /* TODO: piece location hash table */
  /*            rank      file     */
  cell game[SIZE_STD][SIZE_STD];
} board;

board *init_board();
void free_board(board *game);
int move(position origin, position target, board *game);
#endif
