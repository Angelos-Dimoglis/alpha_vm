#include <iostream>
#include <unistd.h>
#include <cassert>
#include <fstream>

#include "../lib/avm_functions.h"
#include "../lib/cpu.h"

using namespace std;

void init_vm() {
}

void cleanup_vm() {
}

int main(int argc, char **argv) {
    bool input_file_set = false;

    char c;
    while ((c = getopt(argc, argv, "i:")) != -1) {
        switch (c) {
            case 'i': // input file

                if (optarg != nullptr)
                    input_file_set = true;

                // open the file

                break;
            default:
                fprintf(stderr, "Unknown option: %s\n", optarg);
                assert(0);
        }
    }

    init_vm();

    // load binary file locally

    //while (!programEnd)
        //execute_cycle();

    cleanup_vm();


    return 0;
}
