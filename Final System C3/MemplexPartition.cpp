//#include "MemplexPartition.h"
//
//void MemplexPartition::memplex_partition() {
//
//    wait(controller.frogs_sent_to_memplex);
//
//
//    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
//         Frog curFrog = controller.all_frogs[i];
//         my_all_frogs.push_back(curFrog);
//    }
//    //out << my_all_frogs[0].solution << "     " << my_all_frogs[0].fitness;
//    controller.memplexes.resize(NUMBER_OF_MEMPLEX);
//    for (int i = 0; i < NUMBER_OF_FROGS; i++) {
//        int memplex_id = i % NUMBER_OF_MEMPLEX;
//        controller.all_frogs[i].memplex_index = memplex_id;
//        controller.all_frogs[i].memplex_offset = controller.memplexes[memplex_id].size();
//        controller.memplexes[memplex_id].push_back(my_all_frogs[i]);
//    }
//
//
//    for (int i = 0; i < NUMBER_OF_MEMPLEX; i++) {
//        for (int j = 0; j < NUMBER_OF_FROGS / NUMBER_OF_MEMPLEX; j++) {
//            std::cout << "[" << controller.memplexes[i][j].fitness << ", " << i << ", " << j << "] Solution: " << controller.memplexes[i][j].solution << "\n";
//        }
//    }
//
//
//    controller.memplex_done_event.notify();
//}