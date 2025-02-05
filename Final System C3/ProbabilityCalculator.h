#pragma once
#pragma once
#include "systemc.h"
#include "Frog.h"
#include "Controller.h"


SC_MODULE(ProbabilityCalculator) {
    sc_in<int> probability_size; //alterantive : use frogNum/mempNum
    sc_fifo_out<double> probability_channel_out;

    std::vector<double> selection_probabilities;

    void compute_probabilities() {
        wait(); //waits for probability_size

        int n = probability_size.read();
        selection_probabilities.clear();
        double sum_pn = 0;

         for (int j = 1; j <= n; j++) {
         double p_n = (2.0 * (n + 1 - j)) / (n * (n + 1));
         selection_probabilities.push_back(p_n);
         sum_pn += p_n;
         }
        //normalize
        for (double &p: selection_probabilities) {
            p /= sum_pn;
        }
        cout << "Shared Probabilities: ";

        //send to channel
        for (double p: selection_probabilities) {
            cout << p << " ";
            probability_channel_out.write(p);
        }
        cout << '\n';
        controller.probability_done.notify();
    }

    SC_CTOR(ProbabilityCalculator) {
        SC_THREAD(compute_probabilities);
        sensitive << probability_size;
    }
};
