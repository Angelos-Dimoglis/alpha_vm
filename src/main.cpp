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
extern unsigned codeSize;

vector<double> numConsts;
vector<string> strConsts;
vector<string> namedLibfuncs = {
    "print",
    "input",
    "objectmemberkeys",
    "objecttotalmembers",
    "objectcopy",
    "totalarguments",
    "argument",
    "typeof",
    "strtonum",
    "sqrt",
    "cos",
    "sin"
};
vector<userfunc*> userFuncs;

vector<instruction> code;

void init_vm() {
}

void cleanup_vm() {
}

void is_valid_binary(ifstream *file) {
    int magic_number = 0;
    string tmp(""); // Helper variable. Simply reads what the number we are reading is, from a comment that is next to it in the binary file.

    *file >> magic_number >> tmp;

    if (file->fail() || magic_number != 69420 || tmp != "magic_number") {
        cerr << "file is not an alpha binary\n";
        assert(0);
    }
}

unsigned int get_string_count(ifstream *file) {
    unsigned int string_consts_count = 0;
    string tmp("");

    *file >> string_consts_count >> tmp;

    if (tmp != "constant_strings" || file->fail()) {
        cerr << "Could not read constant strings\n";
        assert(0);
    }

    string curr = "";
    for (int i = 0; i < string_consts_count; i++) {
        char c;
        
        while (file->get(c) && isspace(c)) {}

        if (c != '"') { //in speciall cases such as a..f(b), the symbol f is stored as a string but 
                        //doesn't have quotes
            file->unget();
            *file >> curr;
        }else {
            while (file->get(c)) {
                if (file->fail()) {
                    cerr << "Could not read constant strings\n";
                    assert(0);
                }
                if (c == '"') break;  // End of string
                curr += c;
            }
        }
        
        strConsts.push_back(curr);
    }

    return string_consts_count;
}

unsigned int get_number_count(ifstream *file) {
    unsigned int number_consts_count = 0;
    string tmp("");

    *file >> number_consts_count >> tmp;

    if (tmp != "constant_numbers" || file->fail()) {
        cerr << "Could not read constant numbers\n";
        assert(0);
    }

    double curr;
    for (int i = 0; i < number_consts_count; i++) {
        *file >> curr;
        if (file->fail()) {
            cerr << "Could not read constant numbers\n";
            assert(0);
        }
        numConsts.push_back(curr);
    }

    return number_consts_count;
}

unsigned int get_user_func_count(ifstream *file) {
    unsigned int user_funcs_count = 0;
    string tmp("");

    *file >> user_funcs_count >> tmp;

    if (tmp != "user_functions" || file->fail()) {
        cerr << "Could not read user functions\n";
        assert(0);
    }

    userfunc *curr = new userfunc;
    for (int i = 0; i < user_funcs_count; i++) {
        *file >> curr->id >> curr->address >> curr->localSize;
        if (tmp != "user_functions" || file->fail()) {
            cerr << "Could not read user functions\n";
            assert(0);
        }
        userFuncs.push_back(curr);
    }

    return user_funcs_count;
}

unsigned int get_library_func_count(ifstream *file) {
    unsigned int library_funcs_count = 0;
    string tmp("");
    *file >> library_funcs_count >> tmp;

    if (tmp != "library_functions" || library_funcs_count != 12 || file->fail()) {
        cerr << "Could not read library functions\n";
        assert(0);
    }

    string curr;
    for (int i = 0; i < library_funcs_count; i++) {
        *file >> curr;
        if (tmp != "library_functions" || file->fail()) {
            cerr << "Could not read library functions\n";
            assert(0);
        }
        assert(namedLibfuncs[i] == curr);
    }

    return library_funcs_count;
}

vmopcode string_to_opcode(string s) {
    vmarg_t type;
    
    if (s == "assign") return assign_v;
    if (s == "add") return add_v;
    if (s == "sub") return sub_v;
    if (s == "mul") return mul_v;
    if (s == "div") return div_v;
    if (s == "mod") return mod_v;
    if (s == "if_eq") return jeq_v;
    if (s == "if_not_eq") return jne_v;
    if (s == "if_less_eq") return jle_v;
    if (s == "if_greater_eq") return jge_v;
    if (s == "if_less") return jlt_v;
    if (s == "if_greater") return jgt_v;
    if (s == "jump") return jump_v;
    if (s == "call") return call_v;
    if (s == "param") return pusharg_v;
    if (s == "funcstart") return funcenter_v;
    if (s == "funcend") return funcexit_v;
    if (s == "newtable") return newtable_v;
    if (s == "tablegetelem") return tablegetelem_v;
    if (s == "tablesetelem") return tablesetelem_v;
    if (s == "nop") return nop_v;

    assert(0);
}

vmarg_t string_to_arg_type(string s) {
    vmarg_t type;
    
    if (s == "label_a") return label_a;
    if (s == "global_a") return global_a;
    if (s == "formal_a") return formal_a;
    if (s == "local_a") return local_a;
    if (s == "number_a") return number_a;
    if (s == "string_a") return string_a;
    if (s == "bool_a") return bool_a;
    if (s == "nil_a") return nil_a;
    if (s == "userfunc_a") return userfunc_a;
    if (s == "libfunc_a") return libfunc_a;
    if (s == "retval_a") return retval_a;

    assert(0);
}

unsigned int get_instruction_count(ifstream *file) {
    unsigned int instructions_count = 0;
    string tmp("");

    *file >> instructions_count >> tmp;

    if (tmp != "instructions" || file->fail()) {
        cerr << "Could not read instructions\n";
        assert(0);
    }

    codeSize = instructions_count;
    instruction curr;
    for (int i = 0; i < instructions_count; i++)  {
        string instr_opcode;
        string arg1_type, arg2_type, result_type;
        unsigned int arg1_val, arg2_val, result_val;
        *file >> instr_opcode >>
        result_type >> result_val >>
        arg1_type >> arg1_val >>
        arg2_type >> arg2_val;

        if (file->fail()) {
            cerr << "Could not read instructions\n";
            assert(0);
        }

        curr.opcode = string_to_opcode(instr_opcode);
        curr.arg1.type = string_to_arg_type(arg1_type);
        curr.arg1.val = arg1_val;
        curr.arg2.type = string_to_arg_type(arg2_type);
        curr.arg2.val = arg1_val;
        curr.result.type = string_to_arg_type(result_type);
        curr.result.val = result_val;

        code.push_back(curr);
    }
    

    return instructions_count;
}

unsigned int get_globals_count(ifstream *file) {
    unsigned int globals_count = 0;
    string tmp("");

    *file >> globals_count >> tmp;
    if (tmp != "num_of_globals" || file->fail()) {
        cerr << "Could not read instructions\n";
        assert(0);
    }

    return globals_count;
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
        cerr << "input file not set\n";
        assert(0);
    }

    ifstream file(file_name);

    if (!file.is_open()) {
        cerr << "error opening the file\n";
        assert(0);
    }

    is_valid_binary(&file);

    unsigned int 
        string_consts_count = get_string_count(&file),
        number_consts_count = get_number_count(&file),
        user_funcs_count    = get_user_func_count(&file),
        library_funcs_count = get_library_func_count(&file),
        globals_count       = get_globals_count(&file),
        instructions_count  = get_instruction_count(&file);

    /*
    cout << endl;
    for (string &s : strConsts)
        cout << s << endl;
    */

    bool print_info = true;
    if (print_info) {
        cout << endl;
        cout << "===== INFO =====" << endl;
        cout << "str: " + to_string(string_consts_count) << endl;
        cout << "num: " + to_string(number_consts_count) << endl;
        cout << "user functions: " + to_string(user_funcs_count) << endl;
        cout << "library functions: " + to_string(library_funcs_count) << endl;
        cout << "globals count: " << globals_count << endl;
        cout << "instructions count: " + to_string(instructions_count) << endl;
        cout << "================" << endl;
        cout << endl;
    }

    file.close();

    // execute the code
    while (!executionFinished) {
        execute_cycle();
    }

    printf("ended execution\n");
    cleanup_vm();

    return 0;
}
