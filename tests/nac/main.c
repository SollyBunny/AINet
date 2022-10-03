  
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ainet.h"

#define b_N 0
#define b_X 5
#define b_O 7
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
	b->turn = b_X;
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
		if (b->turnnum > 7) return b_DRAW;
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

void fight(Net *p) {
	static Board b;
	static int choice = 0;
	static N_TYPE *choices;
	static int outcome;
	b_init(&b);
	while (1) {

		// AI turn
			for (unsigned int i = 0; i < 9; ++i)
				p->in[i] = b.tiles[i] == b_N ? (N_MAX / 2) : (b.tiles[i] == b_X ? N_MAX : 0);

			n_calc(p);
			choices = n_sortout(p);

			choice = 0;
			l_bad_choice:

			outcome = b_domove(&b, choices[choice]);
			switch (outcome) {
				default: break;
				case b_FAIL:
					++choice;
					goto l_bad_choice;
				case b_DRAW:
					b_print(&b);
					printf("Draw\n");
					goto l_end;
				case b_X:
				case b_O:
					b_print(&b);
					printf("Computer wins!\n");
					goto l_end;
			}
			printf("Computer plays: %d\n", choices[choice] + 1);
			b_print(&b);
		// Your turn
			getchar();
			l_bad_player_choice:
			printf("Your turn: ");
			outcome = getchar() - '0' - 1;
			if (outcome < 0 || outcome > 8) goto l_bad_player_choice;
			outcome = b_domove(&b, outcome);
			switch (outcome) {
				default: break;
				case b_FAIL:
					++choice;
					goto l_bad_player_choice;
				case b_DRAW:
					b_print(&b);
					printf("Draw\n");
					goto l_end;
				case b_X:
				case b_O:
					b_print(&b);
					printf("Player wins!\n");
					goto l_end;
			}
			b_print(&b);
	}
	l_end:
	return;
}

int playgame(Net *p1, Net *p2) {
	static Board b;
	static int player = 0;
	static int choice = 0;
	static N_TYPE *choices;
	static int outcome;
	b_init(&b);
	Net *p = p1;
	while (1) {
		for (unsigned int i = 0; i < 9; ++i)
			p->in[i] = b.tiles[i] == b_N ? (N_MAX / 2) : (b.tiles[i] == b_X ? N_MAX : 0);

		n_calc(p);
		choices = n_sortout(p);

		choice = 0;
		l_bad_choice:

		outcome = b_domove(&b, choices[choice]);
		//printf("Choice %d: %d = %d (%d)\n", choice, choices[choice], outcome, b.turnnum);
		switch (outcome) {
			default: break;
			case b_FAIL:
				++choice;
				goto l_bad_choice;
			case b_DRAW:
				return 0;
			case b_X:
			case b_O:
        if (outcome = b_X) return 1;
        return -2;
		}
		//b_print(&b);

		if (player == 0) {
			player = 1;
			p = p2;
		} else {
			player = 0;
			p = p1;
		}
	}
}

int main() {

	srand(time(NULL));

	#define NUMNETS 100
  Nets *net = ng_init(NUMNETS, 9, 9, 2, 20);
	ng_setrand(net);
	for (unsigned int i = 0; i < NUMNETS; ++i) {
		net->net[i]->data = (void*)0;
	}

	#define NUMGEN 1

	int best;
	long bestwins;
	while (1) {

		for (unsigned int _ = 0; _ < NUMGEN; ++_) {
		
			for (unsigned int i = 0; i < NUMNETS; ++i) {
				for (unsigned int m = 0; m < NUMNETS; ++m) {
					static long outcome;
					outcome = (long)playgame(net->net[i], net->net[m]);
          if (outcome == 0)
					net->net[i]->data = (void*)((long)net->net[i]->data + outcome);
					//net->net[m]->data = (void*)((long)net->net[m]->data - outcome);
				}
			}

			best = 0;
			bestwins = (long)net->net[0]->data;
			for (unsigned int i = 1; i < NUMNETS; ++i) {
				if ((long)net->net[i]->data > bestwins) {
					best = i;
					bestwins = (long)net->net[i]->data;
				}
				//printf("Net %d: %ld wins\n", i, (long)net->net[i]->data);
				
			}

			//printf("Best: %d (%ld wins)\n", best, bestwins);
			for (int i = 0; i < NUMNETS; ++i) {
				if (i != best) {
					n_setcopy(net->net[i], net->net[best]);
					n_mutate(net->net[i]);
				}
				net->net[i]->data = (void*)0;
			}
			
		}

		n_save_svg(net->net[best], "out.svg");
		printf("It has been %d games\nBest: %d (%ld wins)\nPick an option:\n 'e': exit! \n 'f': fight the best ai\n Anything else will continue\n>>> ", 
			NUMNETS * NUMNETS * NUMGEN,
			best, bestwins
		);
		switch (getchar()) {
			case 'e':
				goto l_end;
			case 'f':
				fight(net->net[best]);
				break;
			default: break;
		}

	}
	
	l_end:
	ng_free(net);
	return 0;
}

