#ifndef BOARD_H
#define BOARD_H
#include "piece.h"
#define WHITE_CHECK 0x1
#define BLACK_CHECK 0x2
#define FLAG_TURN 0x4

typedef struct {
	/* TODO: piece location hash table */
	/* rank      file     */
	cell		game[SIZE_STD][SIZE_STD];
	uint8_t flags;
} board;

board	       *init_board();
int		is_in(position m, position * moves);
void		free_board(board * game);
int		move_piece(position origin, position target, board * game);
#endif
