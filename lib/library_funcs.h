#include <iostream>
#include "../lib/avm_functions.h"

void libfunc_print() {
    unsigned n = avm_totalactuals();
    for (unsigned i = 0; i < n; i++) {
        cout << avm_getactual(i)->tostring();
    }
}