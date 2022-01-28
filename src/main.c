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
  cbreak();
  WINDOW *win = newwin(SIZE_STD, SIZE_STD, LINES/2 - SIZE_STD/2, COLS/2 - SIZE_STD/2);
  keypad(win, 1);
  box(win, '*', '*');
  int input = 0;
  int sel_row = 0, sel_col = 0;
  int selected = 0;
  position origin;
  position target;
  position *amoves = NULL;
  while (input != 'q') {
	for (int row = 0; row < SIZE_STD; row++) {
	  for (int col = 0; col < SIZE_STD; col++) {
		wmove(win, row, col);
		if (row == sel_row && col == sel_col) {
		  waddch(win, '@');
		} else if (amoves != NULL && is_in(coords_to_pos(row, col), amoves)) {
		  waddch(win, '.');
		} else if (game_board->game[row][col].piece == NULL) {
		  waddch(win, ' ');
		} else {
		  waddch(win, game_board->game[row][col].piece->ident);
		}
	  }
	}
	switch (input = wgetch(win)) {
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
		if (game_board->game[sel_row][sel_col].piece == NULL) break;
		amoves = moves(game_board->game[sel_row][sel_col].piece,coords_to_pos(sel_row, sel_col),game_board->game);
		selected = 1;
	  }
	default:
	  continue;
	}
	wrefresh(win);
  }
  endwin();
  free_board(game_board);
  return 0;
}
