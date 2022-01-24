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
  game->game[0][0] = (moveable){game->rook, WHITE, NORMAL, 0};
  game->game[0][1] = (moveable){game->knight, WHITE, SKIP, 0};
  game->game[0][2] = (moveable){game->bishop, WHITE, NORMAL, 0};
  game->game[0][3] = (moveable){game->queen, WHITE, NORMAL, 0};
  game->game[0][4] = (moveable){game->king, WHITE, NORMAL, 0};
  game->game[0][5] = (moveable){game->bishop, WHITE, NORMAL, 0};
  game->game[0][6] = (moveable){game->knight, WHITE, SKIP, 0};
  game->game[0][7] = (moveable){game->rook, WHITE, NORMAL, 0};

  game->game[7][0] = (moveable){game->rook, BLACK, NORMAL, 0};
  game->game[7][1] = (moveable){game->knight, BLACK, SKIP, 0};
  game->game[7][2] = (moveable){game->bishop, BLACK, NORMAL, 0};
  game->game[7][3] = (moveable){game->queen, BLACK, NORMAL, 0};
  game->game[7][4] = (moveable){game->king, BLACK, NORMAL, 0};
  game->game[7][5] = (moveable){game->bishop, BLACK, NORMAL, 0};
  game->game[7][6] = (moveable){game->knight, BLACK, SKIP, 0};
  game->game[7][7] = (moveable){game->rook, BLACK, NORMAL, 0};

  game->game[1][0] = (moveable){game->pawn, WHITE, NORMAL, 0};
  game->game[1][1] = (moveable){game->pawn, WHITE, NORMAL, 0};
  game->game[1][2] = (moveable){game->pawn, WHITE, NORMAL, 0};
  game->game[1][3] = (moveable){game->pawn, WHITE, NORMAL, 0};
  game->game[1][4] = (moveable){game->pawn, WHITE, NORMAL, 0};
  game->game[1][5] = (moveable){game->pawn, WHITE, NORMAL, 0};
  game->game[1][6] = (moveable){game->pawn, WHITE, NORMAL, 0};
  game->game[1][7] = (moveable){game->pawn, WHITE, NORMAL, 0};

  game->game[6][0] = (moveable){game->pawn, BLACK, NORMAL, 0};
  game->game[6][1] = (moveable){game->pawn, BLACK, NORMAL, 0};
  game->game[6][2] = (moveable){game->pawn, BLACK, NORMAL, 0};
  game->game[6][3] = (moveable){game->pawn, BLACK, NORMAL, 0};
  game->game[6][4] = (moveable){game->pawn, BLACK, NORMAL, 0};
  game->game[6][5] = (moveable){game->pawn, BLACK, NORMAL, 0};
  game->game[6][6] = (moveable){game->pawn, BLACK, NORMAL, 0};
  game->game[6][7] = (moveable){game->pawn, BLACK, NORMAL, 0};
  
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

int move(position origin, position target) {
  /* we will use a position instead of a piece, as we just store
   all the currently utilized cells (or positions) in a hash map,
  with a concatenated version of the rank and file as the key.
  They will just point at the piece they hold, as there is only
  one instance of each piece. We will just have a shared pointer
  between all cells for the same piece, for example, the four
  bishops will just be stored as four entries in the hash map,
  with their position as the hashmap key, and the data they
  point to is the same *bishop variable. */
  ENTRY t;
  t.key = key_from_coords(origin);
  ENTRY *piece = hsearch(t,FIND);
  if (piece == NULL) {
	puts("Illegal position encountered");
	return -1;
  }
  moveable *mov = (moveable *)piece->data;
  /* erase value in the original cell */
  t.data = NULL;
  hsearch(t, ENTER);
  if (mov->mk == NORMAL) {
	position * moves = mov->p->moves(origin); /* we get an array containing all possible moves */
	int move = 0;
	while (moves[move].rank != -1 && moves[move].file != -1) {
	  
	  moves++;
	}
  }
  
}



