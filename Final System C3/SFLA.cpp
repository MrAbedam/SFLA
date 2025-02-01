#include "systemc.h"
#include "SFLA.h"

void SFLA::start() {

	initial_frogs(); //soft
    
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        all_frogs[i].fitness = fitness_function(all_frogs[i].solution);
    }

    fitness_sorter();

    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        for (int j = 0; j < NUMBER_OF_ITEMS; ++j) {
            cout << all_frogs[i].solution[j] << " ";
        }
        cout << '\n' << all_frogs[i].fitness << '\n';
    }


}

//random initial generate solution
void SFLA::initial_frogs() {
    srand(time(0)); //random seed init
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        for (int j = 0; j < NUMBER_OF_ITEMS; ++j) {
            all_frogs[i].solution[j] = (rand()) % 2;
        }
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
