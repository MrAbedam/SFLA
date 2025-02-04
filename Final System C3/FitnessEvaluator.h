#pragma once
#include "systemc.h"
#include "Frog.h"
#include "Controller.h"



SC_MODULE(FitnessEvaluator) {
    sc_fifo_in<sc_bv<NUMBER_OF_ITEMS>> solution_in; 
    sc_fifo_out<int> fitness_out; 

    void compute_fitness(); 

    SC_CTOR(FitnessEvaluator) {
        SC_THREAD(compute_fitness);
    }
};
