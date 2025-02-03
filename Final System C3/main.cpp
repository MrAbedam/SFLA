//#include "systemc.h"
//#include "FrogGeneration.h"
//
//int sc_main(int argc, char* argv[]) {
//    sc_signal<Frog> frog_signal;  // Signal to hold generated frogs
//    sc_signal<bool> valid_signal;  // Signal to indicate frog readiness
//    sc_clock clk("clk", 10, SC_NS);  // 10 ns clock
//
//    FrogGeneration frogGen("frogGen");
//    frogGen.frog_out(frog_signal);
//    frogGen.valid_output(valid_signal);
//    frogGen.clk(clk);
//
//    // Trace file for debugging
//    sc_trace_file* tf = sc_create_vcd_trace_file("FrogGeneration");
//    sc_trace(tf, frog_signal, "FrogSignal");
//    sc_trace(tf, valid_signal, "ValidOutput");
//
//    sc_start(200, SC_NS);  // Run simulation for 200 ns
//    sc_close_vcd_trace_file(tf);
//
//    return 0;
//}


#pragma once
#include "systemc.h"
#include "SFLA.h"
#include "FrogGenerator.h"

int sc_main(int argc, char* argv[]) {
    sc_fifo<Frog> frog_fifo(10); // FIFO buffer for communication

    FrogGenerator frogGen("frogGen");
    SFLA mySFLA("mySFLA");

    // Connect FIFO
    frogGen.frog_out(frog_fifo);
    mySFLA.frog_in(frog_fifo);

    sc_start(10, SC_SEC);
    return 0;
}
