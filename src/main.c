#include "board.h"
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <locale.h>
#include <math.h>

#define WHITE_CELL 1
#define BLUE_CELL 2
#define CHECK_CELL 3

int
main()
{
	board	       *game_board = init_board();
	if (game_board == NULL)
		printf("error initializing board...\n");
	setlocale(LC_ALL, "");
	initscr();
	noecho();
	cbreak();
	start_color();

	init_pair(WHITE_CELL, COLOR_BLACK, COLOR_CYAN);
	init_pair(BLUE_CELL, COLOR_WHITE, COLOR_BLACK);
	init_pair(CHECK_CELL, COLOR_WHITE, COLOR_RED);

	int		min_size = fmin(COLS, LINES);
	int		cell_size = min_size / SIZE_STD;
	int		padding = (COLS - cell_size * SIZE_STD * 2) / 2;

	WINDOW	       *win = newwin(cell_size * SIZE_STD, cell_size * SIZE_STD * 2, 0, padding);
	WINDOW	       *ui_board[SIZE_STD][SIZE_STD];

	keypad(win, 1);
	box(win, 0, 0);
	touchwin(win);
	int		input = 0;
	int		sel_row = 0, sel_col = 0;
	int		selected = 0;
	position	origin;
	position	target;
	position       *amoves = NULL;
	for (int row = 0; row < SIZE_STD; row++) {
		for (int col = 0; col < SIZE_STD; col++) {
			ui_board[row][col] = subwin(win, cell_size, cell_size * 2, row * cell_size, padding + cell_size * col * 2);
			//box(ui_board[row][col], 0, 0);
			wbkgd(ui_board[row][col], COLOR_PAIR(((row + col) % 2 == 0) ? BLUE_CELL : WHITE_CELL));
			touchwin(win);
			if (game_board->game[row][col].piece == NULL)
				continue;
			wmove(ui_board[row][col], cell_size / 2, cell_size / 2);
			waddstr(ui_board[row][col], game_board->game[row][col].piece->pretty);
		}
	}
	while (input != 'q') {
		int		x, y;
		getparyx(ui_board[sel_row][sel_col], y, x);
		wmove(win, y + cell_size / 2, x + cell_size / 2);
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
				int	valid = is_valid(origin, target, game_board,amoves);
				if (valid!=-1) just_move_piece(origin, target, game_board);
				else printf("valid\n");
				for (int i = 0; amoves[i].rank != -1; i++) {
					wmove(ui_board[amoves[i].rank][amoves[i].file], cell_size / 2, cell_size / 2);
					waddstr(ui_board[amoves[i].rank][amoves[i].file],
						game_board->game[amoves[i].rank][amoves[i].file].piece == NULL ? " " :
						game_board->game[amoves[i].rank][amoves[i].file].piece->pretty);

					wrefresh(ui_board[amoves[i].rank][amoves[i].file]);
				}
				free(amoves);
				amoves = NULL;
				selected = 0;
				if (valid == -1)
					break;
				wmove(ui_board[origin.rank][origin.file], cell_size / 2, cell_size / 2);
				waddstr(ui_board[origin.rank][origin.file], " ");

				wmove(ui_board[target.rank][target.file], cell_size / 2, cell_size / 2);
				waddstr(ui_board[target.rank][target.file], game_board->game[target.rank][target.file].piece->pretty);

				wrefresh(ui_board[origin.rank][origin.file]);
				wrefresh(ui_board[target.rank][target.file]);
			} else {
				origin = coords_to_pos(sel_row, sel_col);
				if (game_board->game[sel_row][sel_col].piece == NULL)
					break;
				amoves = moves(game_board->game[sel_row][sel_col].piece, coords_to_pos(sel_row, sel_col), game_board->game, &game_board->game_flags);
				for (int i = 0; amoves[i].rank != -1; i++) {
					wmove(ui_board[amoves[i].rank][amoves[i].file], cell_size / 2, cell_size / 2);
					waddstr(ui_board[amoves[i].rank][amoves[i].file], "o");

					wrefresh(ui_board[amoves[i].rank][amoves[i].file]);
				}
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
