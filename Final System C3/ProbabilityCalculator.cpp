#include "ProbabilityCalculator.h"


void ProbabilityCalculator::compute_probabilities() {
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
    for (double& p : selection_probabilities) {
        p /= sum_pn;
    }
    cout << "Shared Probabilities: ";

    //send to channel
    for (double p : selection_probabilities) {
        cout << p << " ";
        probability_channel_out.write(p);
    }
    cout << '\n';
    controller.probability_done.notify();
}