#pragma once
#include <systemc.h>



#define NUMBER_OF_FROGS 6
#define NUMBER_OF_MEMPLEX 2
#define NUMBER_OF_ITEMS 9
#define Q_SELECTION 2
#define L_MAX_ITERATION 2
#define G_MAX_ITERATION 2
#define EPSILON_CHANGE_UG 1
#define STEP_SIZE_MAX 1

/*
good sfla parameters: (lower stepsize => better result)
#define NUMBER_OF_FROGS 8
#define NUMBER_OF_MEMPLEX 2
#define NUMBER_OF_ITEMS 9
#define Q_SELECTION 2
#define L_MAX_ITERATION 3
#define G_MAX_ITERATION 3
#define EPSILON_CHANGE_UG 0
#define STEP_SIZE_MAX 1
*/


struct Frog {
    sc_bv<NUMBER_OF_ITEMS> solution;
    int fitness;
    int allfrogs_index;
    int memplex_index;
    int memplex_offset;

    Frog(int fit = 0, int idx = 0, int mem_idx = 0, int mem_offset = 0)
        : fitness(fit), allfrogs_index(idx), memplex_index(mem_idx), memplex_offset(mem_offset) {
        solution = 0;
    }

    Frog& operator=(const Frog& rhs) {
        if (this != &rhs) {
            fitness = rhs.fitness;
            allfrogs_index = rhs.allfrogs_index;
            memplex_index = rhs.memplex_index;
            memplex_offset = rhs.memplex_offset;
            solution = rhs.solution;
        }
        return *this;
    }

    bool operator==(const Frog& rhs) const {
        return fitness == rhs.fitness &&
            allfrogs_index == rhs.allfrogs_index &&
            memplex_index == rhs.memplex_index &&
            memplex_offset == rhs.memplex_offset &&
            solution == rhs.solution;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Frog& frog) {
    os << "Fitness: " << frog.fitness
        << ", Index: " << frog.allfrogs_index
        << ", Memplex Index: " << frog.memplex_index
        << ", Memplex Offset: " << frog.memplex_offset
        << ", Solution: " << frog.solution.to_string();
    return os;
}

inline void sc_trace(sc_trace_file* f, const Frog& frog, const std::string& name) {
    sc_trace(f, frog.fitness, name + ".fitness");
    sc_trace(f, frog.allfrogs_index, name + ".allfrogs_index");
    sc_trace(f, frog.memplex_index, name + ".memplex_index");
    sc_trace(f, frog.memplex_offset, name + ".memplex_offset");
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        sc_trace(f, frog.solution[i], name + ".solution[" + std::to_string(i) + "]");
    }
}
