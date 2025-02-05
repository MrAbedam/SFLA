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
    sc_fifo<Frog> sfla_to_ev(NUMBER_OF_FROGS);
    sc_fifo<Frog> ev_to_sfla(NUMBER_OF_FROGS);
    sc_fifo<double> sfla_to_ev_prb(NUMBER_OF_FROGS/NUMBER_OF_MEMPLEX);

    EvolutionModule evolutionModule("evolution");
    FrogGenerator frogGen("frogGen");
    FitnessEvaluator fitnessEval("fitnessEval");
    SFLA mySFLA("mySFLA");

    frogGen.frog_out(frog_fifo);
    mySFLA.frog_in(frog_fifo);

    mySFLA.solution_out(solution_fifo);
    fitnessEval.solution_in(solution_fifo);

    fitnessEval.fitness_out(fitness_fifo);
    mySFLA.fitness_in(fitness_fifo);

    mySFLA.send_to_evolve(sfla_to_ev);
    evolutionModule.receive_form_SFLA(sfla_to_ev);

    mySFLA.send_probs_to_evolve(sfla_to_ev_prb);
    evolutionModule.receive_probs_form_SFLA(sfla_to_ev_prb);

    mySFLA.receive_from_evolve(ev_to_sfla);
    evolutionModule.send_to_SFLA(ev_to_sfla);

    sc_start(10, SC_SEC);
    return 0;
}
