#include <systemc.h>

#define NUMBER_OF_FROGS 12 // number of solutions

#define NUMBER_OF_MEMPLEX 3

#define NUMBER_OF_ITEMS 9 // number of items

#define Q_SELECTION 3 // number of chosen frogs from each memplex for evol

#define L_MAX_ITERATION 2

struct Frog {
	sc_bv<NUMBER_OF_ITEMS> solution;
	int fitness;
	int allfrogs_index;
	int memplex_index;
	int memplex_offset;
};
