#include <iostream>
#include <cstring>
#include "../lib/avm_functions.h"

extern bool executionFinished;

void memclear_string(avm_memcell* m) {
    //assert(holds_alternative<string>(m->data));  // ensure it is a string
    get<string>(m->data).clear();                // clear the contents
}

void memclear_table(avm_memcell* m) {
    //assert(holds_alternative<avm_table>(m->data));
    get<avm_table*>(m->data)->avm_decrefcounter();
}

memclear_func_t memclearFuncs[] = {
    0, /*number*/
    memclear_string,
    0, /*bool*/
    memclear_table,
    0, /*usefunc*/
    0, /*libfunc*/
    0, /*nil*/
    0  /*undef*/
};

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
    cout << format << endl;
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

void avm_error(string format) {
    cerr << format << endl;
    executionFinished = 1;
}

bool number_tobool(avm_memcell* m) {
    return get<double>(m->data) != 0; 
    return true;
}

bool string_tobool(avm_memcell* m) {
    return get<string>(m->data)[0] != 0; 
    return true;
}

bool bool_tobool(avm_memcell* m) {
    return get<bool>(m->data); 
    return true;
}

bool table_tobool(avm_memcell* m) {
    return 1; 
}

bool userfunc_tobool(avm_memcell* m) {
    return 1; 
}

bool libfunc_tobool(avm_memcell* m) {
    return 1; 
}

bool nil_tobool(avm_memcell* m) {
    return 0; 
}

bool undef_tobool(avm_memcell* m) {
    assert(0); // NOTE: ?????
    return 0; 
}

bool avm_tobool(avm_memcell* m) {
    assert(m->type >= 0 && m->type < undef_m);
    return (*toboolFuncs[m->type]) (m);
}
