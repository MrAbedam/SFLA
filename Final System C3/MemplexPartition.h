#pragma once
#include "systemc.h"
#include "Frog.h"
#include "Controller.h"



SC_MODULE(MemplexPartition) {


    void memplex_partition();

    std::vector<Frog> my_all_frogs;

    SC_CTOR(MemplexPartition) {
        SC_THREAD(memplex_partition);
    }
};
