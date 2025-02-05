#include "EvolutionModule.h"
#include <vector>
#include <random>

void EvolutionModule::starter_memplex_evolution() {
    while (true) {
        wait(controller.SFLA_send_data_to_EVOLV);
        receive_data_from_SFLA();
        wait(controller.SFLA_send_data_to_EVOLV_DONE);


        for (int i = 0; i < NUMBER_OF_MEMPLEX; i++) {
            sc_spawn(sc_bind(&EvolutionModule::memplex_evolution, this, i));
        }
        wait(evolves_complte[0] & evolves_complte[1]);


        cout << "Byeee data now! \n";
        printAllFrog();
        send_data_to_SFLA();
        wait(controller.EVOLVE_send_data_to_SFLA_DONE);
        controller.evolvComplete.notify();
    }
}

void EvolutionModule::printAllFrog() {
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

void EvolutionModule::memplex_evolution(int memplex_id) {
    select_q_frogs(memplex_id);
    fitness_sorter(selected_frogs[memplex_id], false);
    evolution_frog(memplex_id);

    // Debugging output
    cout << "M: " << memplex_id << "\nMemplex " << memplex_id << " Evolution Complete.\n";
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        cout << "\nFitness:" << all_frogs[i].fitness << '\n';
        if (all_frogs[i].memplex_index == memplex_id) {
            for (int j = 0; j < NUMBER_OF_ITEMS; ++j) {
                cout << all_frogs[i].solution[j] << " ";
            }
            cout << '\n';
        }
    }
    evolves_complte[memplex_id].notify();
}

void EvolutionModule::receive_data_from_SFLA() {
    all_frogs.clear();
    all_frogs.reserve(NUMBER_OF_FROGS);

    for (int i = 0; i < NUMBER_OF_FROGS; ++i) {
        all_frogs.push_back(receive_form_SFLA.read());
        cout << "Frog " << i << "received " << all_frogs[i].fitness << '\n';
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
    selection_probabilities.clear();
    for (int i = 0; i < NUMBER_OF_FROGS/NUMBER_OF_MEMPLEX; ++i) {
        selection_probabilities.push_back(receive_probs_form_SFLA.read());
    }
    Ug = all_frogs[0];
    controller.SFLA_send_data_to_EVOLV_DONE.notify(SC_ZERO_TIME);
}

void EvolutionModule::send_data_to_SFLA() {
    for (int i = 0; i < NUMBER_OF_FROGS; ++i) {
        send_to_SFLA.write(all_frogs[i]);
    }
    controller.EVOLVE_send_data_to_SFLA_DONE.notify(SC_ZERO_TIME);
}

void EvolutionModule::select_q_frogs(int memplex_id) {

    int n = NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX;
    for (int i = 0; i < n; ++i) {
        cout << selection_probabilities[i] << " ";
    }
    cout << "\n";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<int> dist(&selection_probabilities[0], &selection_probabilities[n - 1]);


    bool selected[NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX] = {false};

    std::cout << "Selected frogs from Memplex " << memplex_id << ": ";
    selected_frogs[memplex_id].clear();
    for (int i = 0; i < Q_SELECTION; i++) {
        int selected_index;
        do {
            selected_index = dist(gen);
        } while (selected[selected_index]); // Ensure unique selection

        selected[selected_index] = true; // Mark this frog as selected
        selected_frogs[memplex_id].push_back(memplexes[memplex_id][selected_index]);

        std::cout << "[Fitness: " << selected_frogs[memplex_id][i].fitness
                  << ", Offset: " << selected_frogs[memplex_id][i].memplex_offset << "] ";
    }
    std::cout << std::endl;
}

void EvolutionModule::evolution_frog(int selected_id) {
    sc_bv<NUMBER_OF_ITEMS> newSolution;
    srand(time(0));
    int current_iteration = 0;

    do {
        fitness_sorter(selected_frogs[selected_id], false);//selected_frogs[0] => Ub

        Frog &Uw = selected_frogs[selected_id][Q_SELECTION - 1];
        cout << "\nold least is:" << Uw.fitness << "  " << Uw.solution << '\n';
        //part1 Uw and Ub
        cout << "//-----------------------------------------//\n";
        if (updateUwBasedOnUb(selected_id)) {
            cout << "\n//-----------------------------------------//\n";
        }

            //part2 Uw and Ug
        else if (updateUwBasedOnUg(selected_id)) {
            cout << "\n//-----------------------------------------//\n";
        }

            //part3 Random
        else {
            for (int j = 0; j < NUMBER_OF_ITEMS; ++j) {
                newSolution[j] = (rand()) % 2;
            }
            cout << "\n         RANDOM USED BY MEMPLEX: " << selected_id;
            setupUwprime(selected_id, newSolution);
        }
        cout << "\n//-----------------------------------------//\n";

        current_iteration++;
        cout << "\nUG FITNESS" << Ug.fitness << "\n";
    } while (current_iteration < L_MAX_ITERATION);
    //Alternate interp: set Ug = all_frogs[0] over here (just in case)
}

void EvolutionModule::setupUwprime(int selected_id, sc_bv<NUMBER_OF_ITEMS> &newSolution) {
    Frog &Uw = selected_frogs[selected_id][Q_SELECTION - 1];

    Uw.solution = newSolution;
    Uw.fitness = fitness_function(newSolution);
    all_frogs[Uw.allfrogs_index].solution = newSolution;
    all_frogs[Uw.allfrogs_index].fitness = Uw.fitness;
    cout << "\nNEW SOLUTION " << Uw.solution << " NEW FITNESS " << Uw.fitness;

}

int EvolutionModule::fitness_function(sc_bv<NUMBER_OF_ITEMS> solution) {
    int value[NUMBER_OF_ITEMS] = {8, 6, 3, 7, 6, 9, 8, 5, 6};
    int weight[NUMBER_OF_ITEMS] = {5, 4, 3, 9, 5, 7, 6, 3, 2};
    int weight_limit = 20;
    int total_value = 0;
    int total_weight = 0;
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        if (solution[i] == 1) {
            total_value += value[i];
            total_weight += weight[i];
        }
    }
    if (total_weight > weight_limit) {
        return -1;
    } else {
        return total_value;
    }
}


bool EvolutionModule::updateUwBasedOnUb(int selected_id) {


    Frog &Uw = selected_frogs[selected_id][Q_SELECTION - 1];
    Frog &Ub = selected_frogs[selected_id][0];

    int random_number = 1;///TODO: ADD RAND TO STEP

    int maxStepSize = STEP_SIZE_MAX;
    int finalMaxStep = std::min(random_number * hamming_distance(Uw.solution, Ub.solution), maxStepSize);
    int curFitness = Uw.fitness;

    sc_bv<NUMBER_OF_ITEMS> newTestSolution;
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        newTestSolution[i] = Uw.solution[i];
    }

    int steps_used = 0;
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        if (Ub.solution[i] != Uw.solution[i]) {
            cout << i << " ";
            newTestSolution[i] = Ub.solution[i]; // Move towards U_B
            steps_used++;
            if (steps_used >= finalMaxStep) {
                cout << "\nMAX STEPS USED!";
                break;
            }
        }
    }

    int newFitness = fitness_function(newTestSolution);


    cout << "\nUb fitness and solution: " << Ub.fitness << "  -  " << Ub.solution;
    cout << "\nUw fitness and solution: " << Uw.fitness << "  -  " << Uw.solution;
    cout << "\nUWprime    and solution: " << newFitness << "  -  " << newTestSolution;

    if (newFitness > curFitness) {
        setupUwprime(selected_id, newTestSolution);
        cout << "\n         UB USED BY MEMPLEX " << selected_id;
        return true;
    }
    return false;
}


bool EvolutionModule::updateUwBasedOnUg(int selected_id) {


    Frog &Uw = selected_frogs[selected_id][Q_SELECTION - 1];

    int random_number = 1;///TODO: ADD RAND TO STEP

    int maxStepSize = STEP_SIZE_MAX;
    int finalMaxStep = std::min(random_number * hamming_distance(Uw.solution, Ug.solution), maxStepSize);
    int curFitness = Uw.fitness;

    sc_bv<NUMBER_OF_ITEMS> newTestSolution;
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        newTestSolution[i] = Uw.solution[i];
    }

    int steps_used = 0;
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        if (Ug.solution[i] != Uw.solution[i]) {
            cout << "\n" << i << " ";
            newTestSolution[i] = Ug.solution[i]; // Move towards Ug
            steps_used++;
            if (steps_used >= finalMaxStep) {
                cout << "\nMAX STEPS USED!!";
                break;
            }
        }
    }

    int newFitness = fitness_function(newTestSolution);


    cout << "\nUg fitness and solution: " << Ug.fitness << "  -  " << Ug.solution;
    cout << "\nUw fitness and solution: " << Uw.fitness << "  -  " << Uw.solution;
    cout << "\nUWprime    and solution: " << newFitness << "  -  " << newTestSolution;

    if (newFitness > curFitness) {
        setupUwprime(selected_id, newTestSolution);
        cout << "\n         UG USED BY MEMPLEX " << selected_id << "\n";
        return true;
    }
    return false;
}

int EvolutionModule::hamming_distance(sc_bv<NUMBER_OF_ITEMS> &U1, sc_bv<NUMBER_OF_ITEMS> &U2) {
    int distance = 0;
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        if (U1[i] != U2[i]) {
            distance++;
        }
    }
    return distance;
}

void EvolutionModule::fitness_sorter(std::vector<Frog> &frogs, bool isAllFrog) {
    std::sort(std::begin(frogs), std::end(frogs), [](const Frog &firstFrog, const Frog &secondFrog) {
        return firstFrog.fitness > secondFrog.fitness;
    });
    if (isAllFrog) {
        for (int i = 0; i < NUMBER_OF_FROGS; i++) {
            all_frogs[i].allfrogs_index = i;
        }
    }
//    Ug = all_frogs[0];
}
