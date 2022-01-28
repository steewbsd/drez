#ifndef PIECE_H
#define PIECE_H
#define SIZE_STD 8
#define SENTINEL coords_to_pos(-1, -1);
#include <stdint.h>

typedef enum { BLACK, WHITE, NONE } SIDE;

typedef struct {
  short rank; /* row */
  short file; /* column */
} position;

typedef struct { 
  char ident; /* FEN style piece identification */
} piece;

typedef struct {
  piece *piece;
  SIDE side;
  uint8_t flags;
} cell;

position *moves(piece *piece, position pos, cell game[SIZE_STD][SIZE_STD]);
extern piece rook, bishop, knight, queen, pawn, king;
position coords_to_pos(short rank, short file);
#endif

