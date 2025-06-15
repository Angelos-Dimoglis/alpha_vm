#include <iostream>
#include <algorithm>
#include <cmath>
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
    else {
        avm_memcell* m = avm_getactual(0);
        if (m->type != table_m) {
            avm_error("type cannot be " + typeStrings[m->type] + " in 'objectmemberkeys'!");
        }
        else {
            avm_memcellclear(&retval);
            retval.type = table_m;
            retval.data = get<avm_table*>(m->data)->memberkeys();
        }
    }
}

void libfunc_objecttotalmembers() {
    unsigned n = avm_totalactuals();

    if (n != 1) {
        avm_error("one argument (not " + to_string(n) + ") expected in 'objecttotalmembers'!");
    }
    else {
        avm_memcell* m = avm_getactual(0);
        if (m->type != table_m) {
            avm_error("type cannot be " + typeStrings[m->type] + " in 'objecttotalmembers'!");
        }
        else {
            avm_memcellclear(&retval);
            retval.type = number_m;
            retval.data = get<avm_table*>(m->data)->totalmembers();
        }
    }
}

void libfunc_objectcopy() {
    unsigned n = avm_totalactuals();

    if (n != 1) {
        avm_error("one argument (not " + to_string(n) + ") expected in 'objectcopy'!");
    }
    else {
        avm_memcell* m = avm_getactual(0);
        if (m->type != table_m) {
            avm_error("type cannot be " + typeStrings[m->type] + " in 'objectcopy'!");
        }
        else {
            avm_memcellclear(&retval);
            retval.type = table_m;
            retval.data = get<avm_table*>(m->data)->copy();
        }
    }
}

void libfunc_totalarguments() {
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    avm_memcellclear(&retval);
    if (!p_topsp) {
        avm_error("'totalarguments' called outside of a function!");
        retval.type = nil_m;
    }
    else {
        retval.type = number_m;
        retval.data = avm_get_envvalue(p_topsp + AVM_NUMACTUALS_OFFSET);
    }
}

void libfunc_argument() {
    unsigned p_topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);
    unsigned n = avm_totalactuals();
    avm_memcellclear(&retval);

    if (!p_topsp) {
        avm_error("'argument' called outside of a function!");
        retval.type = nil_m;
    }
    else {
        if (n != 1) {
            avm_error("one argument (not " + to_string(n) + ") expected in 'argument'!");
        }
        else {
            avm_memcell* m1 = avm_getactual(0);
            if (m1->type != number_m) {
                avm_error("type cannot be " + typeStrings[m1->type] + " in 'argument'!");
            }
            else {
                avm_memcell* m2 = &stack[p_topsp + AVM_STACKENV_SIZE + 1 + get<double>(m1->data)];
                retval.type = m2->type;
                retval.data = m2->data;
            }
        }
    }
}

void libfunc_typeof() {
    unsigned n = avm_totalactuals();

    if (n != 1) {
        avm_error("one argument (not " + to_string(n) + ") expected in 'typeof'!");
    }
    else {
        avm_memcellclear(&retval);
        retval.type = string_m;
        retval.data = typeStrings[avm_getactual(0)->type];
    }
}

void libfunc_strtnum() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("one argument (not " + to_string(n) + ") expected in 'strtnum'!");
    }
    else {
        avm_memcell* m = avm_getactual(0);
        if (m->type != string_m) {
            avm_error("type cannot be " + typeStrings[m->type] + " in 'strtnum'!");
        }
        else {
            avm_memcellclear(&retval);
            try {
                double value = stod(get<string>(m->data));
                retval.type = number_m;
                retval.data = value;
            }
            catch(const invalid_argument& e) {
                retval.type = nil_m;
            }
        }
    }
}

void libfunc_sqrt() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("one argument (not " + to_string(n) + ") expected in 'sqrt'!");
    }
    else {
        avm_memcell* m = avm_getactual(0);
        if (m->type != number_m) {
            avm_error("type cannot be " + typeStrings[m->type] + " in 'sqrt'!");
        }
        else {
            avm_memcellclear(&retval);
            if (get<double>(m->data) < 0) {
                retval.type = nil_m;
            }
            else {
                retval.type = number_m;
                retval.data = sqrt(get<double>(m->data));
            }
        }
    }
}

void libfunc_cos() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("one argument (not " + to_string(n) + ") expected in 'cos'!");
    }
    else {
        avm_memcell* m = avm_getactual(0);
        if (m->type != number_m) {
            avm_error("type cannot be " + typeStrings[m->type] + " in 'cos'!");
        }
        else {
            avm_memcellclear(&retval);
            retval.type = number_m;
            retval.data = cos(get<double>(m->data));
        }
    }
}

void libfunc_sin() {
    unsigned n = avm_totalactuals();
    if (n != 1) {
        avm_error("one argument (not " + to_string(n) + ") expected in 'sin'!");
    }
    else {
        avm_memcell* m = avm_getactual(0);
        if (m->type != number_m) {
            avm_error("type cannot be " + typeStrings[m->type] + " in 'sin'!");
        }
        else {
            avm_memcellclear(&retval);
            retval.type = number_m;
            retval.data = sin(get<double>(m->data));
        }
    }
}