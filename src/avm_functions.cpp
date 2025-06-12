#include <iostream>
#include <cstring>
#include "../lib/avm_functions.h"

void memclear_string(avm_memcell* m) {
    assert(get<string*>(m->data));
    delete get<string*>(m->data);
}

void memclear_table(avm_memcell* m) {
    assert(get<avm_table*>(m->data));
    get<avm_table*>(m->data)->avm_decrefcounter();
}

void avm_memcellclear(avm_memcell* m) {
    if (m->type != undef_m) {
        memclear_func_t f = memclearFuncs[m->type];
        if (f) {
            (*f) (m);
        }
        m->type = undef_m;
    }
}

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
        avm_warning("assigning from 'undef' content!");
    }
    avm_memcellclear(lv);
    
    memcpy(lv, rv, sizeof(avm_memcell));

    if (lv->type == string_m) {
        lv->data = rv->data;
    }
    else if (lv->type == table_m) {
        get<avm_table*>(lv->data)->avm_tableincrefcounter();
    }
}