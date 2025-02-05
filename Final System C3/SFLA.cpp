
#pragma once

#include "SFLA.h"
#include <vector>


void SFLA::start() {
    logProblemInfo();

    cout << "\n\n                 PHASE1 _ INIT FROGS, SET FITNESS AND SORT\n";
    communicateWithSolutionGenerator();
    communicateWithFitnessCal();
    fitness_sorter(all_frogs, true);
    printAllFrog(false);

    cout << "_______________________________________________________________\n"
        << "                   PHASE2 _ SHARED PROBABILITES CALCULCATION\n";
    communicateWithProbCalc();

    ///G MAX FROM HERE
    for (int i = 0; i < G_MAX_ITERATION; i++) {
        fitness_sorter(all_frogs, true);
        Ug = all_frogs[0];

        cout
            << "_______________________________________________________________\n"
            << "                   PHASE3 _ MEMPLEX PARTITION\n";
        memplex_partition();
        printAllFrog(true);


        cout
                << "_______________________________________________________________\n"
                << "                   PHASE4 _ SELECT Q FROGS\n";
        communicateWithEvolution();

        fitness_sorter(all_frogs, true);


        if (abs(Ug.fitness - all_frogs[0].fitness) > EPSILON_CHANGE_UG) {
            i = 0;
        }

        printAllFrog(false);
        cout << "||||||||| iteration " << i  << " ||||||||| " << '\n';
    }
    Ug = all_frogs[0];
    isFinalResult->write(true);
    frogSolution.write(Ug);

}

void SFLA::communicateWithProbCalc() {
    send_allData_to_selection_prob();
    wait(controller.probability_done);
    receive_allData_from_selection_prob();
    wait(controller.probability_received);
}

void SFLA::send_allData_to_selection_prob() {
    int n = NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX;
    probability_size->write(n);//size = size of each memplex
}

void SFLA::receive_allData_from_selection_prob() {
    selection_probabilities.clear();
    for (int i = 0; i < NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX;i++) {
        double curProb;
        probability_channel_in.read(curProb);
        selection_probabilities.push_back(curProb);
    }
    controller.probability_received.notify(SC_ZERO_TIME);
}

void SFLA::communicateWithEvolution() {///EVOLUTION CALCULATOR MODULE
// SFLA SEND TO EVOLUTION MODULE
    send_allData_to_evolve();
    wait(controller.evolvComplete);
    // SFLA WAIT UNTIL EVOLUTION COMPLETE
    receive_allData_from_evolve();
    wait(controller.SFLA_read_EV_data_DONE);
}

void SFLA::communicateWithFitnessCal() {///FITNESS CALCULATOR MODULE
//triggers sent to fitness
    send_to_fitness(all_frogs);
    wait(controller.fitness_done_event);
    //all frogs have fitness and are ready to be received
    receive_fitness(all_frogs);
    wait(controller.fitness_received_event);
    //frogs has been received

}

void SFLA::communicateWithSolutionGenerator() {///SOLUTION GENERATOR MODULE
//generate and receive frogs
    wait(controller.generated_frogs_event);
    receive_init_frogs();
    wait(controller.frogs_received_event);
    //frogs has been received

}

void SFLA::logProblemInfo() const {
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
}

void SFLA::send_allData_to_evolve() {
    //All frog
    for (int i = 0; i < NUMBER_OF_FROGS; ++i) {
        send_to_evolve.write(all_frogs[i]);
        cout << "Frog " << i << " sent successfully" << endl;
    }
    for (int i = 0; i < NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX; ++i) {
        send_probs_to_evolve.write(selection_probabilities[i]);
    }
    controller.SFLA_send_data_to_EVOLV.notify();

}

void SFLA::receive_allData_from_evolve() {
    all_frogs.clear();
    all_frogs.reserve(NUMBER_OF_FROGS);

    for (int i = 0; i < NUMBER_OF_FROGS; ++i) {
        all_frogs.push_back(receive_from_evolve.read());
        cout << "Frog " << i << "received from ev " << all_frogs[i].fitness << '\n';
    }

    for (auto &memplex: memplexes) {
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

void SFLA::fitness_sorter(std::vector<Frog> &frogs, bool isAllFrog) {
    std::sort(std::begin(frogs), std::end(frogs), [](const Frog &firstFrog, const Frog &secondFrog) {
        return firstFrog.fitness > secondFrog.fitness;
    });
    if (isAllFrog) {
        for (int i = 0; i < NUMBER_OF_FROGS; i++) {
            all_frogs[i].allfrogs_index = i;
        }
    }
}


// TODO Create modules for this method
void SFLA::memplex_partition() {
    for (auto &memplex: memplexes) {
        memplex.clear();
    }
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        int memplex_id = i % NUMBER_OF_MEMPLEX;
        all_frogs[i].memplex_index = memplex_id;
        all_frogs[i].memplex_offset = memplexes[memplex_id].size();
        memplexes[memplex_id].push_back(all_frogs[i]);
    }
}

// TODO Create modules for this method
/*void SFLA::compute_selection_probabilities() {
    int n = NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX; // size of each memplex

    selection_probabilities.clear();
    double sum_pn = 0;

    for (int j = 1; j <= n; j++) {
        double p_n = (2.0 * (n + 1 - j)) / (n * (n + 1));
        selection_probabilities.push_back(p_n);
        sum_pn += p_n;
    }
    //normalize
    for (double &p: selection_probabilities) {
        p /= sum_pn;
    }
    cout << "Shared Probabilities: ";
    for (double p: selection_probabilities) {
        cout << p << " ";
    }
    cout << '\n';
}*/


void SFLA::printAllFrog(bool memplexNeeded) {
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        cout << "\nFitness:" << all_frogs[i].fitness << '\n';
        for (int j = 0; j < NUMBER_OF_ITEMS; ++j) {
            cout << all_frogs[i].solution[j] << " ";
        }
        cout << '\n';
    }
    cout << '\n';
    if (memplexNeeded){
        for (int i = 0; i < NUMBER_OF_MEMPLEX; i++) {
            for (int j = 0; j < NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX; j++) {
                std::cout << "[" << memplexes[i][j].fitness << ", " << i << ", " << j << "] Solution: "
                          << memplexes[i][j].solution << "\n";

            }
        }
    }
}