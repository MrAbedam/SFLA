#include "systemc.h"
#include "Frog.h"

SC_MODULE(monitor) {
    sc_in<Frog> FrogSolution;
    sc_in<bool> isFinalResult;

    void monitoring() {
        wait();
        while (!isFinalResult.read()){
            wait();
        };
        cout << "||||||||||||||||||||||||||||||||||||\n";
        cout << "||||||||| Final Solution |||||||||||\n";
        cout << "          " << FrogSolution.read().fitness << " " << FrogSolution.read().solution << endl;
        cout << "||||||||||||||||||||||||||||||||||||\n";

    };

    SC_CTOR(monitor) {
        SC_THREAD(monitoring);
        sensitive << isFinalResult;
    }
};