#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <cassert>
#include <fstream>
#include <string>

#include "../lib/avm_functions.h"
#include "../lib/cpu.h"

using namespace std;

extern bool executionFinished;

extern vector<double> numConsts;
extern vector<string> strConsts;
extern vector<string> namedLibfuncs;
extern vector<userfunc*> userFuncs;

void init_vm() {
}

void cleanup_vm() {
}

int main(int argc, char **argv) {
    bool input_file_set = false;
    string file_name;

    char c;
    while ((c = getopt(argc, argv, "i:")) != -1) {
        switch (c) {
            case 'i': // input file

                if (optarg != nullptr)
                    input_file_set = true;

                file_name = optarg;

                break;
            default:
                fprintf(stderr, "Unknown option: %s\n", optarg);
                assert(0);
        }
    }

    init_vm();

    if (!input_file_set) {
        cout << "input file not set\n";
        assert(0);
    }

    FILE *input_file = fopen(file_name.c_str(), "r"); 
    if (input_file == nullptr) {
        cout << "error opening the file\n";
        assert(0);
    }

    int string_consts_count = 0,
        number_consts_count = 0;

    string separator = "\t";
    int num;
    while (fscanf(input_file, "%d", &num)) {
        // check for magic number
        // constants

        cout << num << endl;
    }

    fclose(input_file);

    while (1);

    // execute the code
    while (!executionFinished)
        execute_cycle();

    cleanup_vm();

    return 0;
}
