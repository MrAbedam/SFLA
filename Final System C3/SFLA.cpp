
#pragma once
#include "SFLA.h"
#include <vector>


void SFLA::start() {
    cout << "\n                           PHASE 0 _ PARAMETERS";
    cout << "\nNUMBER_OF_FROGS " << NUMBER_OF_FROGS;
    cout << "\nNUMBER_OF_MEMPLEX " << NUMBER_OF_MEMPLEX;
    cout << "\nNUMBER_OF_ITEMS " << NUMBER_OF_ITEMS;
    cout << "\nQ_SELECTION " << Q_SELECTION;
    cout << "\nL_MAX_ITERATION " << L_MAX_ITERATION;
    cout << "\nG_MAX_ITERATION " << G_MAX_ITERATION;
    cout << "\nSTEP_SIZE_MAX " << STEP_SIZE_MAX;
    cout << "\nEPSILON_CHANGE_UG " << EPSILON_CHANGE_UG;
    cout << "\n _______________________________________________________________";
    cout << "\n\n                 PHASE1 _ INIT FROGS, SET FITNESS AND SORT\n";


    //generate and receive frogs
    wait(controller.generated_frogs_event);
    receive_init_frogs();
    wait(controller.frogs_received_event);
    //frogs has been received
    send_to_fitness(all_frogs); //triggers sent to fitness 
    wait(controller.fitness_done_event);

    //all frogs have fitness and are ready to be received
    receive_fitness(all_frogs);
    wait(controller.fitness_received_event);

//    cout << "\n\n\nHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";
//
//    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
//        cout << all_frogs[i].fitness << "        " << all_frogs[i].solution<<'\n';
//    }
//
//
//    cout << "HAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n\n\n";
//
//
//    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
//        cout << all_frogs[i].fitness << "    ";
//    }
    
    //modulated till here

    fitness_sorter(all_frogs, true);
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        cout << "\nFitness:" << all_frogs[i].fitness << '\n';
        for (int j = 0; j < NUMBER_OF_ITEMS; ++j) {
            cout << all_frogs[i].solution[j] << " ";
        }
        cout << '\n';
    }

    cout << "_______________________________________________________________\n                   PHASE2 _ MEMPLEX PARTITION\n\n";

    //GMAX FROM HERE

    for (int i = 0; i < G_MAX_ITERATION ; i++){
        fitness_sorter(all_frogs, true);
        Ug = all_frogs[0];

        /*
        send_to_memplex_partition();
        cout << "MANi";
        wait(controller.memplex_done_event);
        cout << "STICK";
        receive_from_memplex_partition(); // tarif she
        wait(controller.memplex_received_event); // hatman notify she dakhel*/

        memplex_partition();


        cout
                << "_______________________________________________________________\n                   PHASE3 _ PROBABILITY CALCULATION\n";
        compute_selection_probabilities();

        cout
                << "_______________________________________________________________\n                   PHASE4 _ SELECT Q FROGS\n";

        cout << "Heyy SFLA SEND TO EVOVLE\n";
        send_allData_to_evolve();
        cout << "SFLA WaitingE\n";
        wait(controller.evolvComplete);
        cout << "SFLA Waiting end\n";
        receive_allData_from_evolve();
        wait(controller.SFLA_read_EV_data_DONE);

        printAllFrog();
        fitness_sorter(all_frogs, true);
        if (abs(Ug.fitness - all_frogs[0].fitness) <= EPSILON_CHANGE_UG){
            // Ug didnt change
        } else{
            i = 0;
        }
        // if change set i to 0
        // if dont change i++
        cout << "|||||||||||||||||||||||||||||| iteration "<< i<<'\n';
    }

    cout << "||||||||||||||||||||||||||||||\n";
    cout << "||||||||| Final Solution |||||||||||\n";
    cout << Ug.fitness << " " << Ug.solution << endl;
    cout << "||||||||||||||||||||||||||||||\n";

    //EVOLVE
}
void SFLA::send_allData_to_evolve(){
    //All frog
    for (int i = 0; i < NUMBER_OF_FROGS; ++i) {
        send_to_evolve.write(all_frogs[i]);
        cout << "Frog " << i << " sent successfully" << endl;
    }
    for (int i = 0; i < NUMBER_OF_FROGS/NUMBER_OF_MEMPLEX; ++i) {
        send_probs_to_evolve.write(selection_probabilities[i]);
    }
    controller.SFLA_send_data_to_EVOLV.notify();

}
void SFLA::receive_allData_from_evolve(){
    all_frogs.clear();
    all_frogs.reserve(NUMBER_OF_FROGS);

    for (int i = 0; i < NUMBER_OF_FROGS; ++i) {
        all_frogs.push_back(receive_from_evolve.read());
        cout << "Frog " << i << "received from ev " << all_frogs[i].fitness << '\n';
    }

    for (auto& memplex : memplexes) {
        memplex.clear();
    }

    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        int memplex_id = i % NUMBER_OF_MEMPLEX;
        all_frogs[i].memplex_index = memplex_id;
        all_frogs[i].memplex_offset = memplexes[memplex_id].size();
        memplexes[memplex_id].push_back(all_frogs[i]);
    }

    Ug = all_frogs[0];

    controller.SFLA_read_EV_data_DONE.notify(SC_ZERO_TIME);

}
void SFLA::send_to_fitness(std::vector<Frog> &all_frogs) {
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        solution_out.write(all_frogs[i].solution);
    }
    controller.frogs_sent_to_fitness.notify();
}

void SFLA::receive_fitness(std::vector<Frog> &all_frogs) {
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        int computed_fitness;
        fitness_in.read(computed_fitness);
        all_frogs[i].fitness = computed_fitness;
    }
    controller.fitness_received_event.notify(SC_ZERO_TIME);
}



void SFLA::receive_init_frogs() {
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        Frog received_frog;
        frog_in.read(received_frog); // Read frog from FIFO
        all_frogs.push_back(received_frog);
    }
    controller.frogs_received_event.notify(SC_ZERO_TIME);
}
int SFLA::fitness_function(sc_bv<NUMBER_OF_ITEMS> solution) {/*
    int value[NUMBER_OF_ITEMS] = { 8, 6, 3, 7, 6, 9, 8, 5, 6 };
    int weight[NUMBER_OF_ITEMS] = { 5, 4, 3, 9, 5, 7, 6, 3, 2 };
    int weight_limit = 20;*/

    int total_value = 0;
    int total_weight = 0;
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        if (solution[i] == 1) {
            total_value += controller.value[i];
            total_weight += controller.weight[i];
        }
    }
    if (total_weight > controller.weight_limit) {
        return -1;
    }
    else {
        return total_value;
    }
}
void SFLA::fitness_sorter(std::vector<Frog>& frogs , bool isAllFrog) {
    std::sort(std::begin(frogs), std::end(frogs), [](const Frog& firstFrog, const Frog& secondFrog) {
        return firstFrog.fitness > secondFrog.fitness;
        });
    if (isAllFrog){
        for (int i = 0; i < NUMBER_OF_FROGS; i++) {
            all_frogs[i].allfrogs_index = i;
        }
    }
//    Ug = all_frogs[0];
}



// TODO
void SFLA::memplex_partition() {
    for (auto& memplex : memplexes) {
        memplex.clear();
    }


    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        int memplex_id = i % NUMBER_OF_MEMPLEX;
        all_frogs[i].memplex_index = memplex_id;
        all_frogs[i].memplex_offset = memplexes[memplex_id].size();
        memplexes[memplex_id].push_back(all_frogs[i]);
    }
    for (int i = 0; i < NUMBER_OF_MEMPLEX; i++) {
        for (int j = 0; j < NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX; j++) {
                std::cout << "[" << memplexes[i][j].fitness << ", " << i << ", " << j << "] Solution: " << memplexes[i][j].solution<<"\n";

        }
    }
}



void SFLA::compute_selection_probabilities() {
    int n = NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX; // size of each memplex

    selection_probabilities.clear();
    double sum_pn = 0;

    for (int j = 1; j <= n; j++) {
        double p_n = (2.0 * (n + 1 - j)) / (n * (n + 1));
        selection_probabilities.push_back(p_n);
        sum_pn += p_n;
    }
    //normalize
    for (double& p : selection_probabilities) {
        p /= sum_pn;
    }
    cout << "Shared Probabilities: ";
    for (double p : selection_probabilities) {
        cout << p << " ";
    }
    cout << '\n';
}

// --------------------
//TODO: HATMAN BE RESET KARDAN SELECTED FROGS HAVASEMUN BASHE 
// --------------------
void SFLA::printAllFrog() {
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        cout << "\nFitness:" << all_frogs[i].fitness << '\n';
        for (int j = 0; j < NUMBER_OF_ITEMS; ++j) {
            cout << all_frogs[i].solution[j] << " ";
        }
        cout << '\n';
    }
    for (int i = 0; i < NUMBER_OF_MEMPLEX; i++) {
        for (int j = 0; j < NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX; j++) {
            std::cout << "[" << memplexes[i][j].fitness << ", " << i << ", " << j << "] Solution: " << memplexes[i][j].solution<<"\n";

        }
    }
}