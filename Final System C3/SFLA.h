#include <systemc.h>

#define NUMBER_OF_FROGS 10 // number of solutions

#define NUMBER_OF_MEMPLEX 2

#define NUMBER_OF_ITEMS 9 // number of items

struct Frog {
	sc_bv<NUMBER_OF_ITEMS> solution;
	int fitness;
};

SC_MODULE(SFLA) {
	//sc_out<bool> valid_output;
	//sc_out<sc_bv<NUMBER_OF_ITEMS>> final_frog_answer;
	Frog all_frogs[NUMBER_OF_FROGS];

	void initial_frogs(); //soft
	int fitness_function(sc_bv<NUMBER_OF_ITEMS> solution); //soft
	void fitness_sorter(); //hard
	void start();

	SC_CTOR(SFLA) {
		SC_METHOD(start);
	}
};