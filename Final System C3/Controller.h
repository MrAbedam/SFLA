#pragma once
#include "systemc.h"
#include "Frog.h"

class Controller {
public:

	int mempNumber;
	sc_event generated_frogs_event; //frogs generated in FrogGenerator and sent to SFLA
	sc_event frogs_received_event; //frogs received from FrogGenerator in SFLA

	sc_event frogs_sent_to_fitness;//frogs sent from SFLA to FitnessEval
	sc_event fitness_done_event;//fitness calculated and sent From FitnessEval to SFLA
	sc_event fitness_received_event; //fitness received from FitnessEval in SFLA

	sc_event frogs_sent_to_memplex; //frogs sent to memplex;
	sc_event memplex_done_event;
	sc_event memplex_received_event;


	std::vector<int> value;
	std::vector<int> weight;
	int weight_limit;
};
extern Controller controller;