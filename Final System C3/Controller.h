#pragma once
#include "systemc.h"

class Controller {
public:
	sc_event generated_frogs_event; //frogs generated in FrogGenerator and sent to SFLA
	sc_event frogs_received_event; //frogs received from FrogGenerator in SFLA

	sc_event frogs_sent_to_fitness;//frogs sent from SFLA to FitnessEval
	sc_event fitness_done_event;//fitness calculated and sent From FitnessEval to SFLA
	sc_event fitness_received_event; //fitness received from FitnessEval in SFLA
};
extern Controller controller;