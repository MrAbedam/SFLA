#pragma once
#include "systemc.h"

class Controller {
public:
	sc_event generated_frogs_event; //frogs generated in FrogGenerator and sent to SFLA
	sc_event frogs_received_event; //frogs received from FrogGenerator in SFLA

	sc_event frogs_sent_to_fitness;//frogs sent from SFLA to FitnessEval
	sc_event fitness_done_event;//fitness calculated and sent From FitnessEval to SFLA
	sc_event fitness_received_event;//fitness received from FitnessEval in SFLA

	sc_event probability_done;
	sc_event probability_received;


    sc_event SFLA_send_data_to_EVOLV; //fitness received from FitnessEval in SFLA
    sc_event evolvComplete; //fitness received from FitnessEval in SFLA
    sc_event SFLA_send_data_to_EVOLV_DONE;

    sc_event EVOLVE_send_data_to_SFLA_DONE;
    sc_event SFLA_read_EV_data_DONE;

    // connection sfla , evole :
	// sc_fifo<int> ev_to_sfla;


	sc_event frogs_sent_to_memplex; //frogs sent to memplex;
	sc_event memplex_done_event;
	sc_event memplex_received_event;


	std::vector<int> value;
	std::vector<int> weight;
	int weight_limit;
};
extern Controller controller;