#include "Frog.h"

// Constructor Implementation
Frog::Frog(int fit, int idx, int mem_idx, int mem_offset)
    : fitness(fit), allfrogs_index(idx), memplex_index(mem_idx), memplex_offset(mem_offset) {
    solution = 0;
}

// Assignment Operator Overload
Frog& Frog::operator=(const Frog& rhs) {
    if (this != &rhs) {
        fitness = rhs.fitness;
        allfrogs_index = rhs.allfrogs_index;
        memplex_index = rhs.memplex_index;
        memplex_offset = rhs.memplex_offset;
        solution = rhs.solution;
    }
    return *this;
}

// Equality Operator Overload
bool Frog::operator==(const Frog& rhs) const {
    return fitness == rhs.fitness &&
        allfrogs_index == rhs.allfrogs_index &&
        memplex_index == rhs.memplex_index &&
        memplex_offset == rhs.memplex_offset &&
        solution == rhs.solution;
}

std::ostream& operator<<(std::ostream& os, const Frog& frog) {
    os << "Fitness: " << frog.fitness
        << ", Index: " << frog.allfrogs_index
        << ", Memplex Index: " << frog.memplex_index
        << ", Memplex Offset: " << frog.memplex_offset
        << ", Solution: " << frog.solution.to_string()
        << std::endl;
    return os;
}

void sc_trace(sc_trace_file*& f, const Frog& frog, std::string name) {
    sc_trace(f, frog.fitness, name + ".fitness");
    sc_trace(f, frog.allfrogs_index, name + ".allfrogs_index");
    sc_trace(f, frog.memplex_index, name + ".memplex_index");
    sc_trace(f, frog.memplex_offset, name + ".memplex_offset");
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        sc_trace(f, frog.solution[i], name + ".solution[" + std::to_string(i) + "]");
    }
}
