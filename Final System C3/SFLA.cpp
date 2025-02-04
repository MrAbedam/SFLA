
#pragma once
#include "SFLA.h"
#include <vector>
#include <random>


std::vector<std::vector<Frog>> memplexes(NUMBER_OF_MEMPLEX);

std::vector<double> selection_probabilities;

std::vector<std::vector<Frog>> selected_frogs(NUMBER_OF_MEMPLEX);


int UgFitnessChange = 10000; // more accurate to condiser sigma of all knapsack items
Frog Ug;


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
    srand(time(0));

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

    cout << "\n\n\nHAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n";

    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        cout << all_frogs[i].fitness << "        " << all_frogs[i].solution<<'\n';
    }


    cout << "HAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n\n\n";


    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        cout << all_frogs[i].fitness << "    ";
    }

    //felan ta modulate beshe
    /*for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        all_frogs[i].fitness = fitness_function(all_frogs[i].solution);
    }*/

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
        memplex_partition();


        cout
                << "_______________________________________________________________\n                   PHASE3 _ PROBABILITY CALCULATION\n";
        compute_selection_probabilities();

        cout
                << "_______________________________________________________________\n                   PHASE4 _ SELECT Q FROGS\n";

        //--------- Paralel for all i's
        for (int i = 0; i < NUMBER_OF_MEMPLEX; i++) {
            sc_spawn(sc_bind(&SFLA::memplex_evolution, this, i));
        }

        wait(evolves_complte[0] & evolves_complte[1]);
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

void SFLA::send_to_fitness(std::vector<Frog> all_frogs) {
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        solution_out.write(all_frogs[i].solution);
    }
    controller.frogs_sent_to_fitness.notify();
}

void SFLA::receive_fitness(std::vector<Frog> all_frogs) {
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        int computed_fitness;
        fitness_in.read(computed_fitness);
        all_frogs[i].fitness = computed_fitness;
    }
    controller.fitness_received_event.notify(SC_ZERO_TIME);
}


void SFLA::memplex_evolution(int memplex_id) {
    select_q_frogs(memplex_id);
    fitness_sorter(selected_frogs[memplex_id], false);
    evolution_frog(memplex_id);

    // Debugging output
    cout << "M: "<<memplex_id << "\nMemplex " << memplex_id << " Evolution Complete.\n";
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

void SFLA::receive_init_frogs() {
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        Frog received_frog;
        frog_in.read(received_frog); // Read frog from FIFO
        all_frogs.push_back(received_frog);
    }
    controller.frogs_received_event.notify(SC_ZERO_TIME);
}


int SFLA::hamming_distance(sc_bv<NUMBER_OF_ITEMS>& U1, sc_bv<NUMBER_OF_ITEMS>& U2) {
    int distance = 0;
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        if (U1[i] != U2[i]) {
            distance++;
        }
    }
    return distance;
}


bool SFLA::updateUwBasedOnUb(int selected_id) {


    Frog& Uw = selected_frogs[selected_id][Q_SELECTION - 1];
    Frog& Ub = selected_frogs[selected_id][0];

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


bool SFLA::updateUwBasedOnUg(int selected_id) {


    Frog& Uw = selected_frogs[selected_id][Q_SELECTION - 1];

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
            cout <<"\n" << i << " ";
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

void SFLA::evolution_frog(int selected_id) {
    sc_bv<NUMBER_OF_ITEMS> newSolution;
   
    int current_iteration = 0;

    do {
        fitness_sorter(selected_frogs[selected_id], false);//selected_frogs[0] => Ub

        Frog& Uw = selected_frogs[selected_id][Q_SELECTION - 1];
        cout << "\nold least is:" << Uw.fitness <<"  "<<Uw.solution << '\n';
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
            cout << "\n         RANDOM USED BY MEMPLEX: "<<selected_id;
            setupUwprime(selected_id, newSolution);
        }
        cout << "\n//-----------------------------------------//\n";

        current_iteration++;
        cout <<"\nUG FITNESS"<<Ug.fitness << "\n";
    } while (current_iteration <= L_MAX_ITERATION);
    //Alternate interp: set Ug = all_frogs[0] over here (just in case)
}

void SFLA::setupUwprime(int selected_id, sc_bv<NUMBER_OF_ITEMS> &newSolution) {
    Frog& Uw = selected_frogs[selected_id][Q_SELECTION - 1];

    Uw.solution = newSolution;
    Uw.fitness = fitness_function(newSolution);
    all_frogs[Uw.allfrogs_index].solution = newSolution;
    all_frogs[Uw.allfrogs_index].fitness = Uw.fitness;
    cout << "\nNEW SOLUTION " << Uw.solution << " NEW FITNESS " << Uw.fitness;

}

int SFLA::fitness_function(sc_bv<NUMBER_OF_ITEMS> solution) {
    int value[NUMBER_OF_ITEMS] = { 8, 6, 3, 7, 6, 9, 8, 5, 6 };
    int weight[NUMBER_OF_ITEMS] = { 5, 4, 3, 9, 5, 7, 6, 3, 2 };
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
void SFLA::select_q_frogs(int memplex_id) {
    int n = NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<int> dist(&selection_probabilities[0], &selection_probabilities[0] + n);

    bool selected[NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX] = { false };

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
