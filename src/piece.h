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

piece rook, bishop, knight, queen, pawn, king;

#endif
