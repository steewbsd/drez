#include "board.h"
#include "piece.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR -1

#define FREE_AND_FAIL(x)                                                       \
  free(x);                                                                     \
  return -1;

board	       *
init_board(char *fen)
{
	board	       *game;
	game = malloc(sizeof(board));
	if (game == NULL)
		exit(1);
	game->game_flags = FLAG_TURN_WHITE;
	/* TODO: interpret FEN notation and initialize game with it */
	/* a standard game will be created as as placeholder */
	game->game[0][0] = (cell) {
		&rook, WHITE, FLAG_FIRSTMOVE
	};
	game->game[0][1] = (cell) {
		&knight, WHITE, FLAG_FIRSTMOVE
	};
	game->game[0][2] = (cell) {
		&bishop, WHITE, FLAG_FIRSTMOVE
	};
	game->game[0][3] = (cell) {
		&queen, WHITE, FLAG_FIRSTMOVE
	};
	game->game[0][4] = (cell) {
		&king, WHITE, FLAG_FIRSTMOVE
	};
	game->game[0][5] = (cell) {
		&bishop, WHITE, FLAG_FIRSTMOVE
	};
	game->game[0][6] = (cell) {
		&knight, WHITE, FLAG_FIRSTMOVE
	};
	game->game[0][7] = (cell) {
		&rook, WHITE, FLAG_FIRSTMOVE
	};

	game->game[7][0] = (cell) {
		&rook, BLACK, FLAG_FIRSTMOVE
	};
	game->game[7][1] = (cell) {
		&knight, BLACK, FLAG_FIRSTMOVE
	};
	game->game[7][2] = (cell) {
		&bishop, BLACK, FLAG_FIRSTMOVE
	};
	game->game[7][3] = (cell) {
		&queen, BLACK, FLAG_FIRSTMOVE
	};
	game->game[7][4] = (cell) {
		&king, BLACK, FLAG_FIRSTMOVE
	};
	game->game[7][5] = (cell) {
		&bishop, BLACK, FLAG_FIRSTMOVE
	};
	game->game[7][6] = (cell) {
		&knight, BLACK, FLAG_FIRSTMOVE
	};
	game->game[7][7] = (cell) {
		&rook, BLACK, FLAG_FIRSTMOVE
	};

	game->game[1][0] = (cell) {
		&pawn, WHITE, FLAG_FIRSTMOVE
	};
	game->game[1][1] = (cell) {
		&pawn, WHITE, FLAG_FIRSTMOVE
	};
	game->game[1][2] = (cell) {
		&pawn, WHITE, FLAG_FIRSTMOVE
	};
	game->game[1][3] = (cell) {
		&pawn, WHITE, FLAG_FIRSTMOVE
	};
	game->game[1][4] = (cell) {
		&pawn, WHITE, FLAG_FIRSTMOVE
	};
	game->game[1][5] = (cell) {
		&pawn, WHITE, FLAG_FIRSTMOVE
	};
	game->game[1][6] = (cell) {
		&pawn, WHITE, FLAG_FIRSTMOVE
	};
	game->game[1][7] = (cell) {
		&pawn, WHITE, FLAG_FIRSTMOVE
	};

	game->game[6][0] = (cell) {
		&pawn, BLACK, FLAG_FIRSTMOVE
	};
	game->game[6][1] = (cell) {
		&pawn, BLACK, FLAG_FIRSTMOVE
	};
	game->game[6][2] = (cell) {
		&pawn, BLACK, FLAG_FIRSTMOVE
	};
	game->game[6][3] = (cell) {
		&pawn, BLACK, FLAG_FIRSTMOVE
	};
	game->game[6][4] = (cell) {
		&pawn, BLACK, FLAG_FIRSTMOVE
	};
	game->game[6][5] = (cell) {
		&pawn, BLACK, FLAG_FIRSTMOVE
	};
	game->game[6][6] = (cell) {
		&pawn, BLACK, FLAG_FIRSTMOVE
	};
	game->game[6][7] = (cell) {
		&pawn, BLACK, FLAG_FIRSTMOVE
	};


	/* init empty cells */
	for (int i = 2; i < 6; i++) {
		for (int j = 0; j < 8; j++) {
			game->game[i][j] = (cell) {
				NULL, NONE, 0
			};
		}
	}
	return game;
}

void
free_board(board * game)
{
	free(game);
}

/*
 * we will use a position instead of a piece, as we just store all the
 * currently utilized cells (or positions) in the game array. They will just
 * point at the piece they hold, as there is only one instance of each piece.
 * We will just have a shared pointer between all cells for the same piece,
 * for example, the four bishops will just be stored as four entries in the
 * array, with their position as the array indices, and the data they point
 * to is the same *bishop variable.
 */
int
validate_move(position origin, position target, board * game, position * amoves)
{
	cell		origin_cell = game->game[origin.rank][origin.file];
	cell		target_cell = game->game[target.rank][target.file];
	int		allocated_by_me = 0;
	/* quick check for turn */
	if (((game->game_flags & FLAG_TURN_WHITE) && origin_cell.side != WHITE) ||
	  (game->game_flags & FLAG_TURN_BLACK && origin_cell.side != BLACK))
		return ERROR;

	/* no piece in the selected cell */
	if (origin_cell.piece == NULL)
		return ERROR;
	/* piece is pinned, so can't move */
	if (origin_cell.flags & FLAG_PIN)
		return ERROR;
	/* check if move would not actually move */
	if (origin.file == target.file && origin.rank == target.rank)
		return ERROR;
	/*
	 * current side has an ongoing check so cannot move (TODO: check if
	 * move would un-check)
	 */
	if ((game->game_flags & FLAG_TURN_BLACK && game->game_flags & FLAG_CHECK_BLACK) ||
	    (game->game_flags & FLAG_TURN_WHITE && game->game_flags & FLAG_CHECK_WHITE))
		return ERROR;

	/* assign passed moves array, else calculate it */
	position       *valid_moves = amoves;
	if (valid_moves == NULL) {
		valid_moves = moves(origin_cell.piece, origin, game->game, &game->game_flags);
		allocated_by_me = 1;
	}

	int		found_move = ERROR;
	found_move = is_in(target, valid_moves);
	if (allocated_by_me)
		free(valid_moves);	/* free move list if WE called the
					 * moves() function */
	return found_move;
}

int
move_piece(position origin, position target, board * game, position * amoves)
{
	cell		origin_cell = game->game[origin.rank][origin.file];
	cell		target_cell = game->game[target.rank][target.file];

	int		valid = validate_move(origin, target, game, amoves);
	if (valid == ERROR)
		return ERROR;
	target_cell.piece =
		origin_cell.piece;	/* set the new position to point to
					 * the piece */
	origin_cell.piece = NULL;	/* erase the piece from the origin */
	target_cell.side = origin_cell.side;	/* copy piece side */
	origin_cell.side = NONE;
	/* toggle flags */
	/* toggle first move to 0 */
	target_cell.flags = origin_cell.flags & ~(FLAG_FIRSTMOVE);
	origin_cell.flags = 0;


	/* TODO: make use of FLAG_FIRSTMOVE to check for pawn movements */
	game->game[target.rank][target.file] = target_cell;
	game->game[origin.rank][origin.file] = origin_cell;

	game->game_flags ^= FLAG_TURN_WHITE;
	game->game_flags ^= FLAG_TURN_BLACK;
	return 0;
}

int
is_in(position m, position * moves)
{
	int		i = 0;
	while (moves[i].file != -1 && moves[i].rank != -1) {
		if (m.file == moves[i].file && m.rank == moves[i].rank)
			return 0;
		i++;
	}
	return -1;
}
