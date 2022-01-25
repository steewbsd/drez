#include "board.h"
#include "piece.h"
#include <search.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

board *init_board(char *fen) {
  board *game;
  game = malloc(sizeof(board));
  if (game == NULL) exit(1);
  
  /* Initialize all template pieces */
  game->bishop = &bishop;
  game->king = &king;
  game->knight = &knight;
  game->queen = &queen;
  game->rook = &rook;
  game->pawn = &pawn;
  /* ------------------------------ */
  game->size_x = SIZE_STD; /* TODO: make game size customizable */
  game->size_y = SIZE_STD;

  
  /* TODO: interpret FEN notation and initialize game with it */
  /* a standard game will be created as as placeholder */
 game->game[0][0] = (moveable){game->rook, WHITE, FLAG_FIRSTMOVE};
 game->game[0][1] = (moveable){game->knight, WHITE, FLAG_FIRSTMOVE};
 game->game[0][2] = (moveable){game->bishop, WHITE, FLAG_FIRSTMOVE};
 game->game[0][3] = (moveable){game->queen, WHITE, FLAG_FIRSTMOVE};
 game->game[0][4] = (moveable){game->king, WHITE, FLAG_FIRSTMOVE};
 game->game[0][5] = (moveable){game->bishop, WHITE, FLAG_FIRSTMOVE};
 game->game[0][6] = (moveable){game->knight, WHITE, FLAG_FIRSTMOVE};
 game->game[0][7] = (moveable){game->rook, WHITE, FLAG_FIRSTMOVE};

 game->game[7][0] = (moveable){game->rook, BLACK, FLAG_FIRSTMOVE};
 game->game[7][1] = (moveable){game->knight, BLACK, FLAG_FIRSTMOVE};
 game->game[7][2] = (moveable){game->bishop, BLACK, FLAG_FIRSTMOVE};
 game->game[7][3] = (moveable){game->queen, BLACK, FLAG_FIRSTMOVE};
 game->game[7][4] = (moveable){game->king, BLACK, FLAG_FIRSTMOVE};
 game->game[7][5] = (moveable){game->bishop, BLACK, FLAG_FIRSTMOVE};
 game->game[7][6] = (moveable){game->knight, BLACK, FLAG_FIRSTMOVE};
 game->game[7][7] = (moveable){game->rook, BLACK, FLAG_FIRSTMOVE};

 game->game[1][0] = (moveable){game->pawn, WHITE, FLAG_FIRSTMOVE};
 game->game[1][1] = (moveable){game->pawn, WHITE, FLAG_FIRSTMOVE};
 game->game[1][2] = (moveable){game->pawn, WHITE, FLAG_FIRSTMOVE};
 game->game[1][3] = (moveable){game->pawn, WHITE, FLAG_FIRSTMOVE};
 game->game[1][4] = (moveable){game->pawn, WHITE, FLAG_FIRSTMOVE};
 game->game[1][5] = (moveable){game->pawn, WHITE, FLAG_FIRSTMOVE};
 game->game[1][6] = (moveable){game->pawn, WHITE, FLAG_FIRSTMOVE};
 game->game[1][7] = (moveable){game->pawn, WHITE, FLAG_FIRSTMOVE};

 game->game[6][0] = (moveable){game->pawn, BLACK, FLAG_FIRSTMOVE};
 game->game[6][1] = (moveable){game->pawn, BLACK, FLAG_FIRSTMOVE};
 game->game[6][2] = (moveable){game->pawn, BLACK, FLAG_FIRSTMOVE};
 game->game[6][3] = (moveable){game->pawn, BLACK, FLAG_FIRSTMOVE};
 game->game[6][4] = (moveable){game->pawn, BLACK, FLAG_FIRSTMOVE};
 game->game[6][5] = (moveable){game->pawn, BLACK, FLAG_FIRSTMOVE};
 game->game[6][6] = (moveable){game->pawn, BLACK, FLAG_FIRSTMOVE};
 game->game[6][7] = (moveable){game->pawn, BLACK, FLAG_FIRSTMOVE};

 /* init empty cells */
 for (int i = 2; i < 7; i++) {
   for (int j = 0; i < 8; j++) {
	 game->game[i][j] = (moveable){NULL, NONE, 0};
   }
 }
 
  return game;
}

void free_board(board *game) {
  free (game);
}

char *key_from_coords(position pos) {
  /* allocate 2, rank and file, which MUST NOT exceed two
	 characters. As a safety measure, we will check it here. */
  if (pos.rank > 9 || pos.file > 9) return NULL;
  char *key = calloc(2, sizeof(char));
  if (key == NULL) return NULL;
  snprintf(key, 2, "%d%d", pos.rank, pos.file);
  return key;
}

int move(position origin, position target, board *game) {
  /* we will use a position instead of a piece, as we just store
   all the currently utilized cells (or positions) in a hash map,
  with a concatenated version of the rank and file as the key.
  They will just point at the piece they hold, as there is only
  one instance of each piece. We will just have a shared pointer
  between all cells for the same piece, for example, the four
  bishops will just be stored as four entries in the hash map,
  with their position as the hashmap key, and the data they
  point to is the same *bishop variable. */
  moveable origin_moveable = game->game[origin.rank][origin.file];
  moveable target_moveable = game->game[target.rank][target.file];
  if ( origin_moveable.piece == NULL) return -1; /* no piece in the selected cell, error */

  if (origin_moveable.flags & FLAG_PIN) return -1; /* piece is pinned, so can't move */
  position *valid_moves = origin_moveable.piece->moves(origin);
  /* check if move is valid by traversing the list of cells the piece is able to move to at this current position */
  int i, found_move = 0;
  while (valid_moves->file != -1 && valid_moves->rank != -1) { /* check for sentinel value (end of array) */
	piece *current_cell_piece = game->game[valid_moves[i].rank][valid_moves[i].file].piece; /* store current cell's piece
																							   (can be NULL) */
	if (current_cell_piece != NULL && current_cell_piece->ident != 'n') return -1; /* if it is not a knight, and pieces are
																		  in the way, block the move */
	if (target.file == valid_moves[i].file && target.rank == valid_moves[i].rank) { /* found a valid move */
	  found_move = 1;
	  break;
	}
	i++;
  }
  if (found_move == 0) return -1; /* no move was found, return */

  target_moveable.piece = origin_moveable.piece; /* set the new position to point to the piece */
  origin_moveable.piece = NULL; /* erase the piece from the origin */
  target_moveable.side = origin_moveable.side; /* copy piece side */
  /* toggle flags */
  /* toggle first move to 0 */
  target_moveable.flags = origin_moveable.flags & ~(FLAG_FIRSTMOVE);
  /* clear up the rest ... */

  /* TODO: make use of FLAG_FIRSTMOVE to check for pawn movements */
  free (valid_moves);
  return 0;
}



