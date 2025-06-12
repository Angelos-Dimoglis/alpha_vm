#include <iostream>
#include "../lib/avm_functions.h"

void avm_warning(string format) {
    cerr << format << endl;
}

void avm_assign(avm_memcell* lv, avm_memcell* rv) {
    if (lv == rv) {
        return;
    }
    if (lv->type == table_m && rv->type == table_m && lv->data == rv->data) {
        return;
    }
    if (rv->type == undef_m) {
        avm_warning("")
    }
}