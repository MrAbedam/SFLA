#include <systemc.h>
#include <ostream>

using namespace sc_core;

struct CUSTOMIZED_TYPE {
    int fit, index;
    int solution[5];

    // Corrected constructor
    CUSTOMIZED_TYPE(int x = 0, int y = 0) : fit(x), index(y) {
        for (int i = 0; i < 5; i++) solution[i] = 0;
    }

    CUSTOMIZED_TYPE& operator=(const CUSTOMIZED_TYPE& rhs) {
        fit = rhs.fit;
        index = rhs.index;
        for (int i = 0; i < 5; i++) {
            solution[i] = rhs.solution[i];
        }
        return *this;
    }

    bool operator==(const CUSTOMIZED_TYPE& rhs) const { // Fixed to be const
        return fit == rhs.fit && index == rhs.index;
    }
};

// Overload stream operator for debugging
std::ostream& operator<<(std::ostream& os, const CUSTOMIZED_TYPE& val) {
    os << "Fit: " << val.fit << ", Index: " << val.index << ", Solution: ";
    for (int i = 0; i < 5; i++) {
        os << val.solution[i];
    }
    os << std::endl;
    return os;
}

// Custom sc_trace function for tracing CUSTOMIZED_TYPE
inline void sc_trace(sc_trace_file*& f, const CUSTOMIZED_TYPE& val, std::string name) {
    sc_trace(f, val.fit, name + ".fit");
    sc_trace(f, val.index, name + ".index");
}

SC_MODULE(FrogGeneration) {
    sc_out<CUSTOMIZED_TYPE> mydog;

    void initial_frogs() {
        wait(2, SC_SEC);  // Wait before generating data
        CUSTOMIZED_TYPE mytest;
        srand(time(0));
        for (int j = 0; j < 5; ++j) {
            mytest.solution[j] = (rand()) % 2;
        }
        mytest.fit = rand() % 100;
        mytest.index = rand() % 10;

        std::cout << "Generated Frog: " << mytest << std::endl;
        mydog.write(mytest);
    }

    SC_CTOR(FrogGeneration) {
        SC_THREAD(initial_frogs);
    }
};

SC_MODULE(SFLA) {
    sc_signal<CUSTOMIZED_TYPE> urdog;
    FrogGeneration myfroggen;

    void read_frogs() {
        while (true) {
            wait(urdog.value_changed_event()); // Wait for signal change
            CUSTOMIZED_TYPE received_frog = urdog.read();
            std::cout << "Received Frog: " << received_frog << std::endl;
        }
    }

    SC_CTOR(SFLA) : myfroggen("myfroggen") {
        myfroggen.mydog(urdog);

        SC_THREAD(read_frogs);
        sensitive << urdog;  // Sensitivity to signal changes
    }
};

int sc_main(int, char* []) {
    SFLA module("module");
    sc_start(10, SC_SEC);
    return 0;
}
