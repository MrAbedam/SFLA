#include <systemc.h>

#define NUMBER_OF_FROGS 12 // number of solutions

#define NUMBER_OF_MEMPLEX 2

#define NUMBER_OF_ITEMS 9 // number of items

#define Q_SELECTION 2 // number of chosen frogs from each memplex for evol

struct Frog {
	sc_bv<NUMBER_OF_ITEMS> solution;
	int fitness;
	int allfrogs_index;
	int memplex_index;
	int memplex_offset;
};
