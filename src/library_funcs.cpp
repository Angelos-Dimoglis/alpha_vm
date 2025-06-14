#include <iostream>
#include <algorithm>
#include "../lib/avm_functions.h"
#include "../lib/cpu.h"

library_func_t libFuncs[] = {
    libfunc_print,
    libfunc_input,
    libfunc_objectmemberkeys,
    libfunc_objecttotalmembers,
    libfunc_objectcopy,
    libfunc_totalarguments,
    libfunc_argument,
    libfunc_typeof,
    libfunc_strtnum,
    libfunc_sqrt,
    libfunc_cos,
    libfunc_sin
};

bool isNumeric(const string& str) {
    return all_of(str.begin(), str.end(), [](char c) {
        return isdigit(static_cast<unsigned char>(c));
    });
}

void libfunc_print() {
    unsigned n = avm_totalactuals();
    for (unsigned i = 0; i < n; i++) {
        cout << avm_getactual(i)->tostring();
    }
}

void libfunc_input() {
    string s;
    cin >> s;
    avm_memcellclear(&retval);
    if (isNumeric(s)) {
        retval.type = number_m;
        retval.data = stod(s);
    }
    else if (s == "true") {
        retval.type = bool_m;
        retval.data = true;
    }
    else if (s == "false") {
        retval.type = bool_m;
        retval.data = false;
    }
    else if (s == "nil") {
        retval.type = nil_m;
    }
    else {
        retval.type = string_m;
        retval.data = s;
    }
}

void libfunc_objectmemberkeys() {
    unsigned n = avm_totalactuals();

    if (n != 1) {
        avm_error("one argument (not " + to_string(n) + ") expected in 'objectmemberkeys'!");
    }
    else if (avm_getactual(n)->type != table_m) {
        avm_error("type cannot be " + typeStrings[avm_getactual(n)->type] + " in 'objectmemberkeys'!");
    }
    else {
        //TODO
        avm_memcell* m = avm_getactual(n);
    }
}