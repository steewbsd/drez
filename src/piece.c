#include "piece.h"
#include <stdlib.h>
#include <string.h>

position coords_to_pos(short rank, short file) {
  position p = {rank, file};
  return p;
}

SIDE opposite(SIDE orig) {
  switch (orig) {
  case WHITE:
    return BLACK;
  case BLACK:
    return WHITE;
  default:
    return NONE;
  }
}

position *rook_valid(position pos, cell game[SIZE_STD][SIZE_STD]) {
  const int max_length =
      SIZE_STD - 1; /* a rook can move ( not counting other pieces ) a total of
                                        size of one side - 1 (to exclude current
                       position) */
  position *
      valid_moves = malloc(sizeof(position) * max_length * 2 +
                           sizeof(position)); /* allocate vertical + horizontal
                                                 moves, intentionally left the +
                                                 sizeof(position) to clarify
                                                 that's reserved for the
                                                 sentinel */
  int move_idx = 0;
  /* Line up */
  for (int row = pos.rank + 1; row < SIZE_STD; row++) {
    valid_moves[move_idx++] = coords_to_pos(row, pos.file);
    if (game[row][pos.file].piece != NULL) 
      break;
  }
  /* Line down */
  for (int row = pos.rank - 1; row >= 0; row--) {
    valid_moves[move_idx++] = coords_to_pos(row, pos.file);
    if (game[row][pos.file].piece != NULL)
      break;
  }
  /* Line left */
  for (int col = pos.file - 1; col >= 0; col--) {
    valid_moves[move_idx++] = coords_to_pos(pos.rank, col);
    if (game[pos.rank][col].piece != NULL)
      break;
  }
  for (int col = pos.file + 1; col < SIZE_STD; col++) {
    valid_moves[move_idx++] = coords_to_pos(pos.rank, col);
    if (game[pos.rank][col].piece != NULL)
      break;
  }
  valid_moves[move_idx] = SENTINEL;
  return valid_moves;
}

position *knight_valid(position pos, cell game[SIZE_STD][SIZE_STD]) {
  const int max_length = 8;
  position *valid_moves =
      malloc(sizeof(position) * max_length + sizeof(position));
  int i, j, move_idx = 0;
  for (i = -2; i <= 2; i += 4) {
    for (j = -1; j <= 1; j += 2) {
      position coords_v = coords_to_pos(pos.rank + i, pos.file + j);
      position coords_h = coords_to_pos(pos.rank + j, pos.file + i);
      if (!(coords_v.file < 0 || coords_v.rank < 0 ||
            coords_v.file > SIZE_STD || coords_v.rank > SIZE_STD)) {
        valid_moves[move_idx++] = coords_v;
      }
      if (!(coords_h.file < 0 || coords_h.rank < 0 ||
            coords_h.file > SIZE_STD || coords_h.rank > SIZE_STD)) {
        valid_moves[move_idx++] = coords_h;
      }
    }
  }
  valid_moves[move_idx] = SENTINEL;
  return valid_moves;
}

position *bishop_valid(position pos, cell game[SIZE_STD][SIZE_STD]) {
  /* we can approximate, in the worst case the bishop will take (board's
   * diagonal length * 2) - 3 */
  const int max_length = SIZE_STD;
  position *valid_moves = malloc(
      sizeof(position) * max_length +
      sizeof(position)); /* allocate vertical + horizontal moves + sentinel */
  int i = 1, move_idx = 0;
  /* first diagonal: top right */
  while (pos.rank + i < SIZE_STD && pos.file + i < SIZE_STD) {
    valid_moves[move_idx++] = coords_to_pos(pos.rank + i, pos.file + i);
    if (game[pos.rank + i][pos.file + i].piece != NULL)
      break;
    i++;
  }
  /* second diagonal: top left */
  i = 1;
  while (pos.rank + i < SIZE_STD && pos.file - i >= 0) {
    valid_moves[move_idx++] = coords_to_pos(pos.rank + i, pos.file - i);
    if (game[pos.rank + i][pos.file - i].piece != NULL)
      break;
    i++;
  }
  /* third diagonal: bottom left */
  i = 1;
  while (pos.rank - i >= 0 && pos.file - i >= 0) {
    valid_moves[move_idx++] = coords_to_pos(pos.rank - i, pos.file - i);
    if (game[pos.rank - i][pos.file - i].piece != NULL)
      break;
    i++;
  }
  /* fourth diagonal: bottom right */
  i = 1;
  while (pos.rank - i < SIZE_STD && pos.file + i < SIZE_STD) {
    valid_moves[move_idx++] = coords_to_pos(pos.rank - i, pos.file + i);
    if (game[pos.rank - i][pos.file + i].piece != NULL)
      break;
    i++;
  }
  valid_moves[move_idx] = SENTINEL;
  return valid_moves;
}

position *queen_valid(position pos, cell game[SIZE_STD][SIZE_STD]) {
  position *bishop_moves = bishop_valid(pos, game);
  position *rook_moves = rook_valid(pos, game);
  int bishop_moves_len, rook_moves_len;
  for (bishop_moves_len = 0; bishop_moves[bishop_moves_len].file != -1;
       bishop_moves_len++)
    ;
  for (rook_moves_len = 0; rook_moves[rook_moves_len].file != -1;
       rook_moves_len++)
    ;
  int total_len = bishop_moves_len + rook_moves_len;
  position *valid_moves =
      malloc(sizeof(position) * total_len + sizeof(position));
  memcpy(valid_moves, bishop_moves, bishop_moves_len);
  memcpy(valid_moves + bishop_moves_len, rook_moves, rook_moves_len);
  valid_moves[bishop_moves_len + rook_moves_len] = SENTINEL;
  return valid_moves;
}

position *king_valid(position pos, cell game[SIZE_STD][SIZE_STD]) {
  const int max_length = 8;
  int move_idx = 0;
  position *valid_moves =
      malloc(sizeof(position) * max_length + sizeof(position));
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
	  if (pos.rank + i < 0 || pos.rank + i >= SIZE_STD || pos.file + i < 0 || pos.file + i >= SIZE_STD) continue;
      if (i == 0 && j == 0) continue;
	  valid_moves[move_idx++] = coords_to_pos(pos.rank + i, pos.file + j);
    }
  }
  valid_moves[move_idx] = SENTINEL;
  return valid_moves;
}

position *pawn_valid(position pos, cell game[SIZE_STD][SIZE_STD]) {
  int max_length = 4;
  position *valid_moves =
      malloc(sizeof(position) * max_length + sizeof(position));
  int move_idx = 0;
  int side_sign = 1;
  if (game[pos.rank][pos.file].side == BLACK) side_sign = -1;
  if (game[pos.rank+side_sign][pos.file].piece == NULL)
	valid_moves[move_idx++] = coords_to_pos(pos.rank + side_sign, pos.file);
  if (game[pos.rank][pos.file].flags & FLAG_FIRSTMOVE) {
    valid_moves[move_idx++] = coords_to_pos(pos.rank + 2*side_sign, pos.file);
  }
  if (game[pos.rank + side_sign][pos.file + side_sign].piece != NULL &&
      game[pos.rank + side_sign][pos.file + side_sign].side ==
          opposite(game[pos.rank][pos.file].side)) {
    valid_moves[move_idx++] = coords_to_pos(pos.rank + side_sign, pos.file + side_sign);
  }
  if (game[pos.rank + side_sign][pos.file - side_sign].piece != NULL &&
      game[pos.rank + side_sign][pos.file - side_sign].side ==
          opposite(game[pos.rank][pos.file].side)) {
    valid_moves[move_idx++] = coords_to_pos(pos.rank + side_sign, pos.file - side_sign);
  }
  valid_moves[move_idx] = SENTINEL;
  return valid_moves;
}

position *moves(piece *piece, position pos, cell game[SIZE_STD][SIZE_STD]) {
  switch (piece->ident) {
  case 'r':
    return rook_valid(pos, game);
    break;
  case 'b':
    return bishop_valid(pos, game);
    break;
  case 'n':
    return knight_valid(pos, game);
    break;
  case 'q':
    return queen_valid(pos, game);
    break;
  case 'k':
    return king_valid(pos, game);
    break;
  case 'p':
    return pawn_valid(pos, game);
    break;
  default:
    return NULL;
  }
}

piece rook = {'r', "♖"};
piece knight = {'n', "♘"};
piece bishop = {'b', "♗"};
piece queen = {'q', "♕"};
piece king = {'k', "♔"};
piece pawn = {'p', "♙"};
