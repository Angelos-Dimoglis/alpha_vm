#include <cassert>

#include "../lib/avm_memcell.h"

#define AVM_STACKENV_SIZE 4
avm_memcell ax, bx, cx;
avm_memcell retval;
unsigned top, topsp;

// TODO: placement of this code might be wrong
/* Reverse translation for constants:
 * getting constant value from index.
 */
double    consts_getnumber  (unsigned index);
char     *consts_getstring  (unsigned index);
char     *libfunc_getused   (unsigned index);
userfunc *userfuncs_getfunc (unsigned index);


// this will be called by the execute_... functions
avm_memcell *avm_translate_operand (vmarg *arg, avm_memcell *reg) {
    switch (arg->type) {
        case global_a:
        case local_a:
        case formal_a:
        case retval_a:
            return &retval;

        case number_a: {
            reg->type = number_m;
            reg->data = consts_getnumber(arg->val);
            return reg; 
        }

        case string_a: {
            reg->type = string_m;
            reg->data = strdup(consts_getstring(arg->val));
            return reg;
        }

        case bool_a: {
            reg->type = bool_m;
            reg->data = arg->val; // TODO: might do a wrong comversion
            return reg;
        }

        case nil_a: {
            reg->type = nil_m;
            return reg;
        }

        case userfunc_a: {
            reg->type = userfunc_m;
            // TODO: see lec 15 slide 10
            reg->data = userfuncs_getfunc(arg->val)->address;
            return reg;
        }

        case libfunc_a: {
            reg->type = libfunc_m;
            reg->data = libfunc_getused(arg->val);
            return reg;
        }

        default: assert(0);
    }
}
