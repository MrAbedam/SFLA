#include "Frog.h"

SC_MODULE(SFLA) {
	//sc_out<bool> valid_output;
	//sc_out<sc_bv<NUMBER_OF_ITEMS>> final_frog_answer;
	//Frog all_frogs[NUMBER_OF_FROGS];

	std::vector<Frog> all_frogs;
	

	void initial_frogs(); //soft

	int fitness_function(sc_bv<NUMBER_OF_ITEMS> solution); //soft

	void fitness_sorter(std::vector<Frog>&frogs); //hard

	void start();

	void memplex_partition();

	void compute_selection_probabilities();

	void select_q_frogs(int memplex_id);


	SC_CTOR(SFLA) {
		SC_METHOD(start);
		all_frogs.reserve(NUMBER_OF_FROGS);
	}
};