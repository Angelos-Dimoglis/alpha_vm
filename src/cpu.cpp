#include <cassert>

#include "../lib/avm_memcell.h"
#include "../lib/avm_instr_set.h"
#include "../lib/cpu.h"

#define AVM_STACKENV_SIZE 4
avm_memcell ax, bx, cx, retval;
unsigned top, topsp;

// TODO: placement of this code might be wrong
/* Reverse translation for constants:
 * getting constant value from index.
 */
double    consts_getnumber  (unsigned index);
char     *consts_getstring  (unsigned index);
char     *libfunc_getused   (unsigned index);
userfunc *userfuncs_getfunc (unsigned index);

unsigned char executionFinished = 0;
unsigned pc = 0;
unsigned currLine = 0;
unsigned codeSize = 0;
struct instruction *code = 0;
#define AVM_ENDING_PC codeSize

execute_func_t executeFuncs [] = {
    execute_assign,
    execute_add,
    execute_sub,
    execute_mul,
    execute_div,
    execute_mod,
    execute_uminus,
    execute_and,
    execute_or,
    execute_not,
    execute_jeq,
    execute_jne,
    execute_jle,
    execute_jge,
    execute_jlt,
    execute_jgt,
    execute_call,
    execute_pusharg,
    execute_funcenter,
    execute_funcexit,
    execute_newtable,
    execute_tablegetelem,
    execute_tablesetelem,
    execute_nop
};

void execute_cycle (void) {
    if (executionFinished)
        return;

    if (pc == AVM_ENDING_PC) {
        executionFinished = 1;
        return;
    }

    assert(pc < AVM_ENDING_PC);
    struct instruction *instr = code + pc;
    assert(instr->opcode >= 0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);
    if (instr->srcLine)
        currLine = instr->srcLine;
    unsigned oldPC = pc;
    (*executeFuncs[instr->opcode])(instr);
    if (pc == oldPC)
        pc++;
}

// this will be called by the execute_... functions
/*
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
*/
