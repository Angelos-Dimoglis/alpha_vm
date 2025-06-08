#include <iostream>
#include <unistd.h>
#include <cassert>
#include <fstream>

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

    // write binary file locally

    return 0;
}
