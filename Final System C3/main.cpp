#pragma once
#include "systemc.h"
#include "SFLA.h"
#include "FrogGenerator.h"
#include "FitnessEvaluator.h"
#include "Controller.h"

int sc_main(int argc, char* argv[]) {
    //initial pop
    sc_fifo<Frog> frog_fifo(10);

    //fitness eval
    sc_fifo<sc_bv<NUMBER_OF_ITEMS>> solution_fifo(10);
    sc_fifo<int> fitness_fifo(10);


    FrogGenerator frogGen("frogGen");
    FitnessEvaluator fitnessEval("fitnessEval");
    SFLA mySFLA("mySFLA");

    frogGen.frog_out(frog_fifo);
    mySFLA.frog_in(frog_fifo);

    mySFLA.solution_out(solution_fifo);
    fitnessEval.solution_in(solution_fifo);

    fitnessEval.fitness_out(fitness_fifo);
    mySFLA.fitness_in(fitness_fifo);


    sc_start(10, SC_SEC);
    return 0;
}
