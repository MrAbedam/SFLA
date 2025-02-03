#pragma once
#include <systemc.h>
#include "Frog.h"

SC_MODULE(FrogGenerator) {
    sc_fifo_out<Frog> frog_out; // FIFO output for sending frogs

    void generate_frogs();

    SC_CTOR(FrogGenerator) {
        SC_THREAD(generate_frogs);
    }
};
