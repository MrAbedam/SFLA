#pragma once
#include "Frog.h"

SC_MODULE(SFLA) {
	//sc_out<bool> valid_output;
	//sc_out<sc_bv<NUMBER_OF_ITEMS>> final_frog_answer;
	//Frog all_frogs[NUMBER_OF_FROGS];

	sc_fifo_in<Frog> frog_in;

	std::vector<Frog> all_frogs;
	
	sc_event received_frogs_event;
	sc_event computed_fitness_event;
	sc_event sorted_fitness_event;
	sc_event partitioned_memplex_event;
	sc_event computed_probabilities_event;
	sc_event selected_q_event;


	//void initial_frogs(); //soft

	int fitness_function(sc_bv<NUMBER_OF_ITEMS> solution); //soft
	void fitness_sorter(std::vector<Frog>&frogs, bool isAllFrog); //hard
	void start();
	void memplex_partition();
	void compute_selection_probabilities();
	void select_q_frogs(int memplex_id);
	void evolution_frog(int selected_id);
	int hamming_distance(sc_bv<NUMBER_OF_ITEMS>&U1, sc_bv<NUMBER_OF_ITEMS>&U2);
	bool updateUwBasedOnUb(int selected_id);
	bool updateUwBasedOnUg(int selected_id);
	void setupUwprime(int selected_id, sc_bv<NUMBER_OF_ITEMS> &newSolution);
	void receive_init_frogs();


	SC_CTOR(SFLA) {
		SC_THREAD(start);
	}

    void memplex_evolution(int memplex_id);
};