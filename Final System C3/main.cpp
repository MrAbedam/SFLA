#pragma once
#include "systemc.h"
#include "SFLA.h"
#include "FrogGenerator.h"
#include "FitnessGenerator.h"

int sc_main(int argc, char* argv[]) {
    SFLA mySFLA("mySFLA");

    FrogGenerator frogGen("frogGen");
    sc_fifo<Frog> frog_fifo_frogGen(NUMBER_OF_FROGS);


    frogGen.frog_out(frog_fifo_frogGen);
    mySFLA.frog_in_from_gen(frog_fifo_frogGen);

    FitnessGenerator fitGen("fitGen");
    sc_fifo<Frog> frog_fifo_fitGen_sendFromSFLA(NUMBER_OF_FROGS);
    sc_fifo<Frog> frog_fifo_fitGen_sendFromFit(NUMBER_OF_FROGS);

    fitGen.frog_in(frog_fifo_fitGen_sendFromSFLA);
    mySFLA.frog_from_SFLA_to_fit(frog_fifo_fitGen_sendFromSFLA);

    fitGen.frog_out(frog_fifo_fitGen_sendFromFit);
    mySFLA.frog_from_fit_to_SFLA(frog_fifo_fitGen_sendFromFit);


    sc_start(10, SC_SEC);
    return 0;
}
