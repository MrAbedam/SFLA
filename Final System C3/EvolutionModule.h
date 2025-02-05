#pragma once

#include "systemc.h"
#include "Frog.h"
#include "Controller.h"


SC_MODULE(EvolutionModule) {

    sc_fifo_in<Frog> receive_form_SFLA;

    sc_fifo_in<double> receive_probs_form_SFLA;

    sc_fifo_out<Frog> send_to_SFLA;

    std::vector<Frog> all_frogs;

    std::vector<std::vector<Frog>> memplexes;

    std::vector<double> selection_probabilities;

    std::vector<std::vector<Frog>> selected_frogs;

    int UgFitnessChange = 1000;

    Frog Ug;

    sc_event evolves_complte[NUMBER_OF_MEMPLEX];

    void memplex_evolution(int memplex_id);

    void starter_memplex_evolution();

    void evolution_frog(int selected_id);

    void select_q_frogs(int memplex_id);

    void setupUwprime(int selected_id, sc_bv<NUMBER_OF_ITEMS> &newSolution);

    int fitness_function(sc_bv<NUMBER_OF_ITEMS> solution);

    bool updateUwBasedOnUb(int selected_id);

    bool updateUwBasedOnUg(int selected_id);

    int hamming_distance(sc_bv<NUMBER_OF_ITEMS> &U1, sc_bv<NUMBER_OF_ITEMS> &U2);

    void fitness_sorter(std::vector<Frog> &frogs, bool isAllFrog);

    void receive_data_from_SFLA();

    void send_data_to_SFLA();

    SC_CTOR(EvolutionModule) : all_frogs(NUMBER_OF_FROGS),
                               memplexes(NUMBER_OF_MEMPLEX),
                               selection_probabilities(NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX),
                               selected_frogs(NUMBER_OF_MEMPLEX) {
        int siz = NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX;
        for (int i = 0; i < NUMBER_OF_MEMPLEX; ++i) {
            memplexes.push_back(std::vector<Frog>(siz));
        }
        for (int i = 0; i < NUMBER_OF_MEMPLEX; ++i) {
            selected_frogs.push_back(std::vector<Frog>(Q_SELECTION));
        }

        SC_THREAD(starter_memplex_evolution);
    }

    void printAllFrog();
};