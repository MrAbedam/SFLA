#include "FitnessGenerator.h"

void FitnessGenerator::generate_fitness() {
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        Frog curFrog = frog_in.read();
        curFrog.fitness = fitness_function(curFrog.solution);
        curFrog.allfrogs_index = i;
        frog_out.write(curFrog);
    }
}

int FitnessGenerator::fitness_function(sc_bv<NUMBER_OF_ITEMS> solution) {
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
    return (total_weight > weight_limit) ? -1 : total_value;
}
