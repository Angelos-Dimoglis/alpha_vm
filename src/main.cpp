#include <iostream>
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

    // load binary file locally
    instruction *code;

    if (input_file_set) {
        string text;
        ifstream my_stream(file_name);
        while (getline(my_stream, text))
            cout << text << endl;
    } else {
        cout << "file not set\n";
        assert(0);
    }


    while (!executionFinished) {
        ;
    }

    cleanup_vm();


    return 0;
}
