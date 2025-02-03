#pragma once

#include <systemc.h>
#include "Frog.h"

SC_MODULE(FitnessGenerator) {
    sc_fifo_in<Frog> frog_in;
    sc_fifo_out<Frog> frog_out;

    int fitness_function(sc_bv<NUMBER_OF_ITEMS> solution);

    void generate_fitness();  // Declare function, don't define it here!

    SC_CTOR(FitnessGenerator) {
        SC_THREAD(generate_fitness);
    }
};
