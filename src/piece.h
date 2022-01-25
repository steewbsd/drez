#ifndef PIECE_H
#define PIECE_H
#define SIZE 8

typedef struct {
  short rank; /* row */
  short file; /* column */
} position;

typedef struct { 
  char ident; /* FEN style piece identification */
  position *(*moves)(position); /* Legal moves, without game context, for current piece */
} piece;

extern piece rook, bishop, knight, queen, pawn, king;
position coords_to_pos(short rank, short file);
#endif
