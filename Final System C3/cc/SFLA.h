#include "Frog.h"

SC_MODULE(SFLA) {
	//sc_out<bool> valid_output;
	//sc_out<sc_bv<NUMBER_OF_ITEMS>> final_frog_answer;
	//Frog all_frogs[NUMBER_OF_FROGS];
    sc_signal<Frog> urdog;
    FrogGeneration myfroggen;
	std::vector<Frog> all_frogs;
	

	void initial_frogs(); //soft

	int fitness_function(int solution[NUMBER_OF_ITEMS]); //soft

	void fitness_sorter(std::vector<Frog>&frogs); //hard

	void start();

	void memplex_partition();

	void compute_selection_probabilities();

	void select_q_frogs(int memplex_id);

	void evolution_frog(int selected_id);

	int hamming_distance(int U1[NUMBER_OF_ITEMS] , int U2[NUMBER_OF_ITEMS]);

	bool updateUwBasedOnUb(int selected_id);

	bool updateUwBasedOnUg(int selected_id);

	void setupUwprime(int selected_id, int newSolution[NUMBER_OF_ITEMS] );



	SC_CTOR(SFLA) : myfroggen("myfroggen") {
        myfroggen.mydog(urdog);
        SC_THREAD(start);
		all_frogs.reserve(NUMBER_OF_FROGS);
        sensitive << urdog;
	}
};