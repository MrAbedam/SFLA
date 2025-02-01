#include "systemc.h"
#include "SFLA.h"
#include <vector>
#include <random>


std::vector<std::vector<Frog>> memplexes(NUMBER_OF_MEMPLEX);

std::vector<double> selection_probabilities;


Frog selected_frogs[NUMBER_OF_MEMPLEX][Q_SELECTION]; // Stores selected frogs per memplex

void SFLA::start() {

    initial_frogs(); //soft

    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        all_frogs[i].fitness = fitness_function(all_frogs[i].solution);
    }

    fitness_sorter();

    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        //cout << "\nIndex:" << all_frogs[i].frog_index << "\nFitness:" << all_frogs[i].fitness << '\n';
        cout << "\nFitness:" << all_frogs[i].fitness << '\n';
        for (int j = 0; j < NUMBER_OF_ITEMS; ++j) {
            cout << all_frogs[i].solution[j] << " ";
        }
        cout << '\n';
    }

    cout << '\n';

    memplex_partition();

    compute_selection_probabilities();

    cout << selection_probabilities[all_frogs[2].memplex_offset];

}

void SFLA::initial_frogs() {
    //srand(time(0)); //random seed init
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        for (int j = 0; j < NUMBER_OF_ITEMS; ++j) {
            all_frogs[i].solution[j] = (rand()) % 2;
        }
        all_frogs[i].allfrogs_index = i;
    }
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


void SFLA::fitness_sorter() {
    std::sort(std::begin(all_frogs), std::end(all_frogs), [](const Frog& firstFrog, const Frog& secondFrog) {
        return firstFrog.fitness > secondFrog.fitness;
        });
}




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

    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        std::cout << "[" << all_frogs[i].fitness << ", " << all_frogs[i].memplex_index << ", " << all_frogs[i].memplex_offset << "]\n";
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

    for (int i = 0; i < Q_SELECTION; i++) {
        int selected_index;
        do {
            selected_index = dist(gen);
        } while (selected[selected_index]); // Ensure unique selection

        selected[selected_index] = true; // Mark this frog as selected
        selected_frogs[memplex_id][i] = memplexes[memplex_id][selected_index];

        std::cout << "[Fitness: " << selected_frogs[memplex_id][i].fitness
            << ", Offset: " << selected_frogs[memplex_id][i].memplex_offset << "] ";
    }
    std::cout << std::endl;
}
