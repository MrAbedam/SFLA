#pragma once
#include "FitnessEvaluator.h"
#include <cstdlib>
#include <ctime>
#include "Controller.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


void FitnessEvaluator::compute_fitness() {
    wait(controller.frogs_sent_to_fitness);

#if _WIN32
    std::ifstream file("items_data.txt");
#else
    std::ifstream file("/Users/mamadhosein/Documents/Code/KNTU uni/Hardware_Software_CoDesign/SFLA-/Final System C3/items_data.txt");
#endif

    std::string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            std::stringstream ss(line);
            std::string label;
            ss >> label;

            if (label == "Values:") {
                int v;
                while (ss >> v) controller.value.push_back(v);
            }
            else if (label == "Weights:") {
                int w;
                while (ss >> w) controller.weight.push_back(w);
            }
            else if (label == "Weight_Limit:") {
                ss >> controller.weight_limit;
            }
        }
        file.close();
    }
    else {
        std::cerr << "Error: Unable to open file!" << std::endl;
        return;
    }

    if (controller.value.size() != controller.weight.size()) {
        std::cerr << "Error: Mismatch in value and weight sizes!" << std::endl;
        return;
    }

    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        sc_bv<NUMBER_OF_ITEMS> solution;
        solution_in.read(solution);

        int total_value = 0, total_weight = 0;
        for (int j = 0; j < NUMBER_OF_ITEMS; j++) {
            if (solution[j] == 1) {
                total_value += controller.value[j];
                total_weight += controller.weight[j];
            }
        }
       
        int fitness = (total_weight > controller.weight_limit) ? -1 : total_value;
        fitness_out.write(fitness);
    }

    controller.fitness_done_event.notify();
}
