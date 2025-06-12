#include <assert.h>
#include "../lib/avm_memcell.h"

typedef void (*memclear_func_t) (avm_memcell*);

void memclear_string(avm_memcell* m);
void memclear_table(avm_memcell* m);

memclear_func_t memclearFuns[] = {
    0, /*number*/
    memclear_string,
    0, /*bool*/
    memclear_table,
    0, /*usefunc*/
    0, /*libfunc*/
    0, /*nil*/
    0  /*undef*/
};

void avm_warning(string format);

void avm_assign(avm_memcell* lv, avm_memcell* rv);
