#pragma once
#include "FitnessEvaluator.h"
#include <cstdlib>
#include <ctime>
#include "Controller.h"

void FitnessEvaluator::compute_fitness() {
    wait(controller.frogs_sent_to_fitness);
    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        sc_bv<NUMBER_OF_ITEMS> solution;
        solution_in.read(solution);


        int value[NUMBER_OF_ITEMS] = { 8, 6, 3, 7, 6, 9, 8, 5, 6 };
        int weight[NUMBER_OF_ITEMS] = { 5, 4, 3, 9, 5, 7, 6, 3, 2 };
        int weight_limit = 20;
        int total_value = 0, total_weight = 0;

        for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
            if (solution[i] == 1) {
                total_value += value[i];
                total_weight += weight[i];
            }
        }

        int fitness = (total_weight > weight_limit) ? -1 : total_value;
        fitness_out.write(fitness);
    }
    controller.fitness_done_event.notify();
}
