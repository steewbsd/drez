#include "board.h"
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <math.h>

int main() {
  board *game_board = init_board();
  if (game_board == NULL) printf("error initializing board...\n");
  initscr();
  noecho();
  keypad(stdscr, 1);
  int input = 0;
  int sel_row = 0, sel_col = 0;
  int selected = 0;
  position origin;
  position target;
  position *amoves = NULL;
  while (input != 'q') {
	for (int row = 0; row < SIZE_STD; row++) {
	  for (int col = 0; col < SIZE_STD; col++) {
		move(row, col);
		if (row == sel_row && col == sel_col) {
		  addch('@');
		} else if (amoves != NULL && is_in(coords_to_pos(row, col), amoves)) {
		  addch('.');
		} else if (game_board->game[row][col].piece == NULL) {
		  addch(' ');
		} else {
		  addch(game_board->game[row][col].piece->ident);
		}
	  }
	}
	switch (input = getch()) {
	case KEY_UP:
	  sel_row = fmin(sel_row + 1, SIZE_STD - 1);
	  break;
	case KEY_DOWN:
	  sel_row = fmax(0, sel_row - 1);
	  break;
	case KEY_LEFT:
	  sel_col = fmax(0, sel_col - 1);
	  break;
	case KEY_RIGHT:
	  sel_col = fmin(SIZE_STD - 1, sel_col + 1);
	  break;
	case 'c':
	  if (selected) {
		target = coords_to_pos(sel_row, sel_col);
		move_piece(origin, target, game_board);
		free (amoves);
		amoves = NULL;
		selected = 0;
	  } else {
		origin = coords_to_pos(sel_row, sel_col);
		amoves = moves(game_board->game[sel_row][sel_col].piece,coords_to_pos(sel_row, sel_col),game_board->game);
		selected = 1;
	  }
	default:
	  continue;
	}
	refresh();
  }
  endwin();
  free_board(game_board);
  return 0;
}
