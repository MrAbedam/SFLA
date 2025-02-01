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

SC_MODULE(SFLA) {
	//sc_out<bool> valid_output;
	//sc_out<sc_bv<NUMBER_OF_ITEMS>> final_frog_answer;
	Frog all_frogs[NUMBER_OF_FROGS];


	void initial_frogs(); //soft

	int fitness_function(sc_bv<NUMBER_OF_ITEMS> solution); //soft

	void fitness_sorter(); //hard

	void start();

	void memplex_partition();

	void compute_selection_probabilities();

	void select_q_frogs(int memplex_id);


	SC_CTOR(SFLA) {
		SC_METHOD(start);
	}
};