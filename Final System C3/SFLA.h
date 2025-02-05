#pragma once

#include "Frog.h"
#include "Controller.h"
#include "EvolutionModule.h"

SC_MODULE(SFLA) {

    //Data
    std::vector<Frog> all_frogs;
    std::vector<std::vector<Frog>> memplexes;
    std::vector<double> selection_probabilities;
    Frog Ug;


    // Connect to FrogGen
    sc_fifo_in<Frog> frog_in;

    //Connect to Fitness
    sc_fifo_out<sc_bv<NUMBER_OF_ITEMS>> solution_out; // Sending solutions to FitnessEvaluator
    sc_fifo_in<int> fitness_in; // Receiving fitness values from FitnessEvaluator

    //Connect to Probability
    sc_out<int> probability_size;
    sc_fifo_in<double> probability_channel_in;

    //Connect to Evolution
    sc_fifo_out<Frog> send_to_evolve;
    sc_fifo_out<double> send_probs_to_evolve;
    sc_fifo_in<Frog> receive_from_evolve;

    //Connect to monitor
    sc_out<Frog> frogSolution;
    sc_out<bool> isFinalResult;

    void start();

    void receive_init_frogs();

    void send_to_fitness(std::vector<Frog> &all_frogs);

    void receive_fitness(std::vector<Frog> &all_frogs);

    void fitness_sorter(std::vector<Frog> &frogs, bool isAllFrog);

    void memplex_partition();

    void send_allData_to_selection_prob();

    void receive_allData_from_selection_prob();

    void send_allData_to_evolve();

    void receive_allData_from_evolve();

    SC_CTOR(SFLA) : memplexes(NUMBER_OF_MEMPLEX) {
        SC_THREAD(start);
    }

    void printAllFrog(bool memplexNeeded);

    void logProblemInfo() const;

    void communicateWithSolutionGenerator();

    void communicateWithFitnessCal();

    void communicateWithEvolution();

    void communicateWithProbCalc();
};