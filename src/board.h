#ifndef BOARD_H
#define BOARD_H

#include "piece.h"
#define SIZE_STD 8

enum FILE_NUMS { a = 0, b = 1, c = 2, d = 3, e = 4, f = 5, g = 6, h = 7 };
enum SIDE { BLACK, WHITE };
enum MOVE_KIND { NORMAL, SKIP }; /* signals whether a piece (namely the knight) can ignore other
									pieces in its path */

typedef struct {
  piece *rook;
  piece *bishop;
  piece *knight;
  piece *queen;
  piece *pawn;
  piece *king;
  /* custom sizes are a programming experiment, they probably will get
	 removed later, as it certainly makes no sense (just as a fun chaotic game) */
  int size_x; /* 8, in standard mode */
  int size_y; /* 8, in standard mode */
  /* TODO: piece location hash table */
} board;

typedef struct {
  piece *p;
  SIDE s;
  MOVE_KIND mk;
} moveable;

board *init_board();
char *key_from_coords(position pos);
int move(position origin, position target);
#endif
