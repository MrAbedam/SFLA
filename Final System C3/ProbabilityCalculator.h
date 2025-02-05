#pragma once
#pragma once
#include "systemc.h"
#include "Frog.h"
#include "Controller.h"


SC_MODULE(ProbabilityCalculator) {
    sc_in<int> probability_size; //alterantive : use frogNum/mempNum
    sc_fifo_out<double> probability_channel_out;

    std::vector<double> selection_probabilities;

    void compute_probabilities(); 

    SC_CTOR(ProbabilityCalculator) {
        SC_THREAD(compute_probabilities);
        sensitive << probability_size;
    }
};
