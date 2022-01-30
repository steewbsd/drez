#ifndef PIECE_H
#define PIECE_H
#include <stdint.h>

#define SIZE_STD 8
#define SENTINEL coords_to_pos(-1, -1);
#define FLAG_PIN       0x1	/* block piece when it would discover a check */
#define FLAG_CHECK     0x2	/* block moves when king is in check
				 * (additional checking should be done for
				 * moves preventing check) */
#define FLAG_FIRSTMOVE 0x4	/* for castles and pawn starts */


typedef enum {
	BLACK, WHITE, NONE
} SIDE;

typedef struct {
	short		rank;	/* row */
	short		file;	/* column */
} position;

typedef struct {
	char		ident;	/* FEN style piece identification */
	char	       *pretty;
} piece;

typedef struct {
	piece	       *piece;
	SIDE		side;
	uint8_t		flags;
} cell;

position *	moves(piece * piece, position pos, cell game[SIZE_STD][SIZE_STD]);
extern piece rook, bishop, knight, queen, pawn, king;
position	coords_to_pos(short rank, short file);
#endif
