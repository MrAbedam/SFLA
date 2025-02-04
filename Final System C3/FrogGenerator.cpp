#pragma once
#include "FrogGenerator.h"
#include <cstdlib>
#include <ctime>
#include "Controller.h"

void FrogGenerator::generate_frogs() {
    srand(time(0)); 

    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
        Frog curFrog;
        for (int j = 0; j < NUMBER_OF_ITEMS; ++j) {
            curFrog.solution[j] = rand() % 2; 
        }
        curFrog.allfrogs_index = i;
        frog_out.write(curFrog);
    }
    controller.generated_frogs_event.notify(SC_ZERO_TIME);
}
