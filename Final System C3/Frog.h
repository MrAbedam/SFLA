#include <systemc.h>

#define NUMBER_OF_FROGS 12 // number of solutions

#define NUMBER_OF_MEMPLEX 3

#define NUMBER_OF_ITEMS 9 // number of items

#define Q_SELECTION 3 // number of chosen frogs from each memplex for evol

#define L_MAX_ITERATION 10

#define STEP_SIZE_MAX 1

struct Frog {
	sc_bv<NUMBER_OF_ITEMS> solution;
	int fitness;
	int allfrogs_index;
	int memplex_index;
	int memplex_offset;
};

/*
void convert_from_frog_vector(const std::vector<Frog>& frogs, sc_bv<NUMBER_OF_ITEMS> solutions[], int fitness[],
    int allfrogs_index[], int memplex_index[], int memplex_offset[]) {
    int size = frogs.size();

    for (int i = 0; i < size; i++) {
        solutions[i] = frogs[i].solution;
        fitness[i] = frogs[i].fitness;
        allfrogs_index[i] = frogs[i].allfrogs_index;
        memplex_index[i] = frogs[i].memplex_index;
        memplex_offset[i] = frogs[i].memplex_offset;
    }
}

#include <vector>

// Convert arrays of parameters into a vector of Frog objects
std::vector<Frog> convert_to_frog_vector(sc_bv<NUMBER_OF_ITEMS> solutions[], int fitness[], int allfrogs_index[],
    int memplex_index[], int memplex_offset[], int size) {
    std::vector<Frog> frogs;

    for (int i = 0; i < size; i++) {
        Frog f;
        f.solution = solutions[i];
        f.fitness = fitness[i];
        f.allfrogs_index = allfrogs_index[i];
        f.memplex_index = memplex_index[i];
        f.memplex_offset = memplex_offset[i];

        frogs.push_back(f);
    }

    return frogs;
}*/