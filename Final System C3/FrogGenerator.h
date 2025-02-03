#pragma once
#include <systemc.h>
#include "Frog.h"

SC_MODULE(FrogGenerator) {
    sc_fifo_out<Frog> frog_out;

    void generate_frogs();

    SC_CTOR(FrogGenerator) {
        SC_THREAD(generate_frogs);
    }
};
