#pragma once
#include "systemc.h"
#include "SFLA.h"
#include "FrogGenerator.h"

int sc_main(int argc, char* argv[]) {
    sc_fifo<Frog> frog_fifo(10);

    FrogGenerator frogGen("frogGen");
    SFLA mySFLA("mySFLA");

    frogGen.frog_out(frog_fifo);
    mySFLA.frog_in(frog_fifo);

    sc_start(10, SC_SEC);
    return 0;
}
