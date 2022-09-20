
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ainet.h"

#define b_N 0
#define b_X 4
#define b_O 9
#define b_NONE 0
#define b_DRAW -1
#define b_FAIL -2
typedef struct {
	int tiles[9];
	int turn;
	int turnnum;
} Board;

void b_init(Board *b) {
	for (unsigned int i = 0; i < 9; ++i) b->tiles[i] = b_N;
	b->turn = 0;
	b->turnnum = 0;
}

int b_domove(Board *b, int m) {
	if (b->tiles[m] != b_N) return b_FAIL;
	b->tiles[m] = b->turn;
	if (b->turnnum > 4) {
		// i know this is bad, i'm lazy
		int turncondition = b->turn * 3;
		if (
			// Horizontal
			b->tiles[0] + b->tiles[1] + b->tiles[2] == turncondition ||
			b->tiles[3] + b->tiles[4] + b->tiles[5] == turncondition ||
			b->tiles[6] + b->tiles[7] + b->tiles[8] == turncondition ||
			// Vertical
			b->tiles[0] + b->tiles[3] + b->tiles[6] == turncondition ||
			b->tiles[1] + b->tiles[4] + b->tiles[7] == turncondition ||
			b->tiles[2] + b->tiles[5] + b->tiles[8] == turncondition ||
			// Diag
			b->tiles[0] + b->tiles[4] + b->tiles[8] == turncondition ||
			b->tiles[2] + b->tiles[4] + b->tiles[6] == turncondition
		) return b->turn;
		if (b->turnnum > 8) return b_DRAW;
	};
	b->turn = b->turn == b_X ? b_O : b_X;
	++(b->turnnum);
	return b_NONE;
}

void b_print(Board *b) {
	printf("%c%c%c\n%c%c%c\n%c%c%c\n",
		b->tiles[0] == b_N ? ' ' : (b->tiles[0] == b_X ? 'X' : 'O'),
		b->tiles[1] == b_N ? ' ' : (b->tiles[1] == b_X ? 'X' : 'O'),
		b->tiles[2] == b_N ? ' ' : (b->tiles[2] == b_X ? 'X' : 'O'),
		b->tiles[3] == b_N ? ' ' : (b->tiles[3] == b_X ? 'X' : 'O'),
		b->tiles[4] == b_N ? ' ' : (b->tiles[4] == b_X ? 'X' : 'O'),
		b->tiles[5] == b_N ? ' ' : (b->tiles[5] == b_X ? 'X' : 'O'),
		b->tiles[6] == b_N ? ' ' : (b->tiles[6] == b_X ? 'X' : 'O'),
		b->tiles[7] == b_N ? ' ' : (b->tiles[7] == b_X ? 'X' : 'O'),
		b->tiles[8] == b_N ? ' ' : (b->tiles[8] == b_X ? 'X' : 'O')
	);
}

int main() {

	srand(time(NULL));

	Net *p1 = n_init(9, 9, 3, 7);
	n_setrand(p1);

	Net *p2 = n_init(9, 9, 3, 7);
	n_setrand(p2);

	int gamesnum  = 0;
	int gamesx    = 0;
	int gameso    = 0;
	int gamesdraw = 0;

	int player = 0;
	int choice;
	int outcome;
	N_TYPE *choices;
	Net *p;

	Board b;

	l_start:
	#define GAMESTODO 50000
	if (gamesnum == GAMESTODO) {
		printf("It has been %d games\nOutcomes: X: %d O: %d Draw: %d\nPick an option:\n 'e': exit! \n 's': save the neurel network to out.svg\n Anything else will continue\n>>> ",
			gamesnum,
			gamesx, gameso, gamesdraw
		);
		gamesnum  = 0;
		gamesx    = 0;
		gameso    = 0;
		gamesdraw = 0;
		switch (getchar()) {
			case 'e':
				goto l_end;
			case 's':
				n_save_svg(p, "out.svg");
				break;
			default: break;
		}
		
	} else ++gamesnum;

	b_init(&b);

	while (1) {

		if (player == 0) p = p1;
		else p = p2;

		for (unsigned int i = 0; i < 9; ++i)
			p->in[i] = b.tiles[i] == b_N ? (N_MAX / 2) : (b.tiles[i] == b_X ? N_MAX : 0);

		n_calc(p);
		choices = n_sortout(p);

		choice = 0;
		l_bad_choice:

		outcome = b_domove(&b, choices[choice]);
		switch (outcome) {
			default: break;
			case b_DRAW:
				++gamesdraw;
				player = player == 1 ? 0 : 1;
				n_mutate(p);
				goto l_start;
			case b_FAIL:
				++choice;
				goto l_bad_choice;
			case b_X:
			case b_O:
				if (outcome == b_X) ++gamesx;
				else ++gameso;
				if (player == 0) {
					n_setcopy(p1, p2);
					n_mutate(p1);
					player = 1;
				} else {
					n_setcopy(p2, p1);
					n_mutate(p2);
					player = 0;
				}
				goto l_start;
				
		}

		player = player == 1 ? 0 : 1;
		//b_print(&b);
		
	}

	l_end:

	n_free(p1);
	n_free(p2);
	
	return 0;
}

