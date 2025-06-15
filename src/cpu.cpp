#include <cassert>
#include <cstdio>
#include <iostream>
#include "../lib/avm_memcell.h"
#include "../lib/avm_instr_set.h"
#include "../lib/cpu.h"

avm_memcell ax, bx, cx, retval;
unsigned top, topsp;

avm_memcell stack[AVM_STACK_SIZE];

//Consts are read from main
extern vector<double> numConsts;
extern vector<string> strConsts;
extern vector<string> namedLibfuncs;
extern vector<userfunc*> userFuncs;

bool executionFinished = false;
unsigned pc = 0;
unsigned currLine = 0;
unsigned codeSize = 0;
extern vector<instruction> code;

execute_func_t executeFuncs[] = {
    execute_assign,
    execute_add,
    execute_sub,
    execute_mul,
    execute_div,
    execute_mod,
    execute_jmp,
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
    if (executionFinished) {
        printf("finished\n");
        return;
    }

    if (pc == AVM_ENDING_PC) {
        executionFinished = true;
        printf("reached the end\n");
        return;
    }

    assert(pc < AVM_ENDING_PC);
    struct instruction *instr = code.data() + pc;
    assert(instr->opcode >= 0 && instr->opcode <= AVM_MAX_INSTRUCTIONS);
    if (instr->srcLine)
        currLine = instr->srcLine;
    unsigned oldPC = pc;
    (*executeFuncs[instr->opcode])(instr);
    if (pc == oldPC)
        pc++;

    cout << "cycle complete, opcode: " << instr->opcode << endl;
}

// this will be called by the execute_... functions
avm_memcell* avm_translate_operand(vmarg* arg, avm_memcell* reg) {
    if (arg->type == label_a)
        assert(0);

    if (arg->type >= 4 && arg->type <= 9)
        assert(reg != nullptr);

    switch (arg->type) {
        case global_a: return &stack[AVM_STACK_SIZE - 1 - arg->val];
        case local_a:  return &stack[topsp - arg->val];
        case formal_a: return &stack[topsp + AVM_STACKENV_SIZE + 1 + arg->val];
        case retval_a: return &retval;

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
            reg->data = arg->val; // TODO: might do a wrong conversion
            return reg;
        }

        case nil_a: {
            reg->type = nil_m;
            return reg;
        }

        case userfunc_a: {
            reg->type = userfunc_m;
            // TODO: see lec 15 slide 10
            reg->data = arg->val;
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
