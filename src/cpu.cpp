#include <cassert>
#include "../lib/cpu.h"
#include "../lib/avm_memcell.h"
#include "../lib/avm_instr_set.h"
#include "../lib/cpu.h"

#define AVM_STACKENV_SIZE 4
avm_memcell ax, bx, cx, retval;
unsigned top, topsp;

extern vector<double> numConsts;
extern vector<string> strConsts;
extern vector<string> namedLibfuncs;
extern vector<userfunc*> userFuncs;

unsigned char executionFinished = 0;
unsigned pc = 0;
unsigned currLine = 0;
unsigned codeSize = 0;
struct instruction *code = 0;
#define AVM_ENDING_PC codeSize

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

inline vector<avm_memcell> stack;

// this will be called by the execute_... functions
avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg) {
    switch (arg->type) {
        case global_a:
            return &stack[stack.max_size() - 1 - arg->val];
        case local_a:
            return &stack[topsp - arg->val];
        case formal_a:
            return &stack[topsp + stack.size() + 1 + arg->val];
        case retval_a:
            return &retval;

        case number_a: {
            reg->type = number_m;
            reg->data = numConsts[arg->val];
            return reg; 
        }

        case string_a: {
            reg->type = string_m;
            reg->data = strConsts[arg->val];
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
            reg->data = userFuncs[arg->val]->address;
            return reg;
        }

        case libfunc_a: {
            reg->type = libfunc_m;
            reg->data = namedLibfuncs[arg->val];
            return reg;
        }

        default: assert(0);
    }
}
*/
