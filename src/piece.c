#include "piece.h"
#include <stdlib.h>

position coords_to_pos(short rank, short file) {
  position p = {rank, file};
  return p;
}

position *rook_valid (position pos) {
  const int max_length = SIZE - 1; /* a rook can move ( not counting other pieces ) a total of
							   size of one side - 1 (to exclude current position) */
  position *valid_moves = malloc(sizeof(position)*max_length*2+sizeof(position)); /* allocate vertical + horizontal moves,
																				   intentionally left the + sizeof(position) to
																				  clarify that's reserved for the sentinel */
  int i, move_idx = 0;
  for (i = 0; i <= max_length; i++) {
	if (pos.file == i) continue; /* skip current position */
	valid_moves[move_idx++] = coords_to_pos(pos.rank, i);
  }
  for (i = 0; i <= max_length; i++) {
	if (pos.rank == i) continue; /* skip current position */
	valid_moves[move_idx++] = coords_to_pos(i, pos.file);
  }
  valid_moves[move_idx] = coords_to_pos(-1, -1); /* Sentinel value */
  return valid_moves;
}

position *knight_valid(position pos) {
  return NULL;
}

position *bishop_valid (position pos) {
  /* we can approximate, in the worst case the bishop will take (board's diagonal length * 2) - 3 */
  const int max_length = SIZE;
  position *valid_moves = malloc(sizeof(position)*max_length+sizeof(position)); /* allocate vertical + horizontal moves + sentinel */
  int i = 0;
  int min = (pos.file < pos.rank) ? pos.file : pos.rank;
  position lower_left = {pos.rank - min, pos.file - min};
  /* now we start crawling diagonally up right */
  int n = 0;
  while (lower_left.rank+i < SIZE && lower_left.file+i < SIZE) {
	if (pos.rank == lower_left.rank+i && pos.file == lower_left.file+i) continue;
	valid_moves[n++] = coords_to_pos(lower_left.rank+i, lower_left.file+i);
	i++;
  }
  /* TODO: other diagonal */
  valid_moves[n] = coords_to_pos(-1, -1);
  return valid_moves;
  //  rank_dif *= ((rank_dif>0) - (rank_dif<0)); /* get abs value */
}

position *queen_valid (position pos) {
  return NULL;
}

position *king_valid(position pos) {
  return NULL;
}

position *pawn_valid (position pos) {
  position *valid_moves = malloc(sizeof(position)*3+sizeof(position)); /* three possible moves: forwards, twice forwards (at start), en passant */
  // TODO
  return valid_moves;
}

piece rook = {'r', &rook_valid};
piece knight = {'n', &knight_valid};
piece bishop = {'b',&bishop_valid};
piece queen = {'q', &queen_valid};
piece king = {'k', &king_valid};
piece pawn = {'p', &pawn_valid};
