#include <iostream>
#include <cstring>

#include "../lib/avm_functions.h"
#include "../lib/cpu.h"

extern vector<string> namedLibfuncs;
extern vector<userfunc*> userFuncs;

unsigned totalActuals = 0;

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
    assert(0); // NOTE: is this valid?
    return 0; 
}

bool avm_tobool(avm_memcell* m) {
    assert(m->type >= 0 && m->type < undef_m);
    return (*toboolFuncs[m->type]) (m);
}

void avm_call_functor(avm_table* t) {
    cx.type = string_m;
    cx.data = "()";
    avm_memcell* f = t->avm_tablegetelem(cx);
    if (!f) {
        avm_error("in calling table: n '()' element found!");
    }
    else if (f->type == table_m) {
        avm_call_functor(get<avm_table*>(f->data));
    }
    else if (f->type == userfunc_m) {
        avm_push_table_arg(t);
        avm_callsaveenvironment();
        pc = get<unsigned>(f->data);
        assert(pc < AVM_ENDING_PC && code[pc].opcode == funcenter_v);
    }
    else {
        avm_error("in calling table: illegal '()' elemnt value!");
    }
}

void avm_dec_top() {
    if (!top)
        avm_error("stack overflow");
    else
        top--;
}

void avm_push_envvalue(unsigned val) {
    stack[top].type = number_m;
    stack[top].data = val;
    avm_dec_top();
}

void avm_callsaveenvironment() {
    avm_push_envvalue(totalActuals);
    assert(code[pc].opcode == call_v);
    avm_push_envvalue(pc+1);
    avm_push_envvalue(top + totalActuals + 2);
    avm_push_envvalue(topsp);
}

unsigned avm_get_envvalue(unsigned i) {
    assert(stack[i].type == number_m);
    unsigned val = get<unsigned>(stack[i].data);
    assert(get<unsigned>(stack[i].data) == ((double) val));
    return val;
}

void avm_push_table_arg(avm_table* t) {
    stack[top].type = table_m;
    stack[top].data = t;
    get<avm_table*>(stack[top].data)->avm_tableincrefcounter();
    totalActuals++;
    avm_dec_top();
}


userfunc *avm_getfuncinfo(unsigned i) {
    return userFuncs[i];
}

library_func_t avm_getlibraryfunc(string id) {
    for (unsigned i = 0; i < namedLibfuncs.size(); i++) {
        if (id == namedLibfuncs[i]) {
            return libFuncs[i];
        }
    }
    return nullptr;
}

void avm_calllibfunc(string id) {
    library_func_t f = avm_getlibraryfunc(id);
    if (!f) {
        avm_error("unsopported lib func " + id + " called!");
    } else {
        avm_callsaveenvironment();
        topsp = top;
        totalActuals = 0;
        (*f)();
        if (!executionFinished) {
            execute_funcexit((instruction*) 0);
        }
    }
}

unsigned avm_totalactuals() {
    return avm_get_envvalue(topsp + AVM_NUMACTUALS_OFFSET);
}

avm_memcell* avm_getactual(unsigned i) {
    assert(i < avm_totalactuals());
    return &stack[topsp + AVM_STACKENV_SIZE + 1 + i];
}
