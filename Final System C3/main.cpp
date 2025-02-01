#include "SFLA.h"
#include "systemc.h"

int sc_main(int argc, char* argv[]) {
    SFLA mySFLA("my_sfla");
    sc_start(10, SC_SEC);
    return 0;
}
