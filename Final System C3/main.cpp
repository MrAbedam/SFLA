#pragma once
#include "systemc.h"
#include "SFLA.h"
#include "FrogGenerator.h"
#include "FitnessEvaluator.h"
#include "monitor.h"
#include "ProbabilityCalculator.h"

int sc_main(int argc, char* argv[]) {
    //initial pop
    sc_fifo<Frog> frog_fifo(NUMBER_OF_FROGS);

    //fitness eval
    sc_fifo<sc_bv<NUMBER_OF_ITEMS>> solution_fifo(NUMBER_OF_FROGS);
    sc_fifo<int> fitness_fifo(NUMBER_OF_FROGS);
    sc_fifo<Frog> sfla_to_ev(NUMBER_OF_FROGS);
    sc_fifo<Frog> ev_to_sfla(NUMBER_OF_FROGS);
    sc_fifo<double> sfla_to_ev_prb(NUMBER_OF_FROGS/NUMBER_OF_MEMPLEX);
    sc_fifo<double> probability_channel;

    sc_signal<Frog> frogFinalResult;
    sc_signal<bool> isFinalResult;
    sc_signal<int>  probabilty_size;

    EvolutionModule evolutionModule("evolution");
    FrogGenerator frogGen("frogGen");
    FitnessEvaluator fitnessEval("fitnessEval");
    ProbabilityCalculator probCalc("probCalc");
    SFLA sfla("sfla");
    monitor monitor("monitorFinalResult");


    frogGen.frog_out(frog_fifo);
    sfla.frog_in(frog_fifo);

    sfla.solution_out(solution_fifo);
    fitnessEval.solution_in(solution_fifo);

    fitnessEval.fitness_out(fitness_fifo);
    sfla.fitness_in(fitness_fifo);

    //connect
    sfla.probability_channel_in(probability_channel);
    probCalc.probability_channel_out(probability_channel);
    sfla.probability_size(probabilty_size);
    probCalc.probability_size(probabilty_size);
    

    sfla.send_to_evolve(sfla_to_ev);
    evolutionModule.receive_form_SFLA(sfla_to_ev);

    sfla.send_probs_to_evolve(sfla_to_ev_prb);
    evolutionModule.receive_probs_form_SFLA(sfla_to_ev_prb);

    sfla.receive_from_evolve(ev_to_sfla);
    evolutionModule.send_to_SFLA(ev_to_sfla);

    monitor.isFinalResult(isFinalResult);
    monitor.FrogSolution(frogFinalResult);
    sfla.isFinalResult(isFinalResult);
    sfla.frogSolution(frogFinalResult);
    sc_start(10, SC_SEC);
    return 0;
}
