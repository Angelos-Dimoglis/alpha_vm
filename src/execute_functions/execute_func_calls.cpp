#include "../../lib/avm_functions.h"
#include "../../lib/cpu.h"

extern unsigned totalActuals;

void execute_call(instruction* instr) {
    avm_memcell* func = avm_translate_operand(&instr->result, &ax);
    assert(func);
    switch (func->type) {
        case userfunc_m: {
            avm_callsaveenvironment();
            pc = get<unsigned>(func->data);
            assert(pc < AVM_ENDING_PC);
            assert(code[pc].opcode == funcenter_v);
            break;
        }

        case string_m:
        case libfunc_m:
            avm_calllibfunc(get<string>(func->data));
            break;
        case table_m:
            avm_call_functor(get<avm_table*>(func->data));
            break;

        default: {
            avm_error("call: cannot bind " + func->tostring() + " to function!");
        }
    }
}

//TODO: Return userfunc with localSize and check if right amount

void execute_funcenter(instruction* instr) {
    avm_memcell* func = avm_translate_operand(&instr->result, &ax);
    assert(func);
    assert(pc == get<unsigned>(func->data));

    totalActuals = 0;
    userfunc* funcInfo = avm_getfuncinfo(pc);
    topsp = top;
    top = top - funcInfo->localSize;
}

void execute_funcexit(instruction* unused) {
    unsigned oldTop = top;
    top = avm_get_envvalue(topsp + AVM_SAVEDTOP_OFFSET);
    pc = avm_get_envvalue(topsp + AVM_SAVEDPC_OFFSET);
    topsp = avm_get_envvalue(topsp + AVM_SAVEDTOPSP_OFFSET);

    while (++oldTop <= top)
        avm_memcellclear(&stack[oldTop]);
}

void execute_pusharg(instruction* instr) {
    avm_memcell* arg = avm_translate_operand(&instr->arg1, &ax);
    assert(arg);

    avm_assign(&stack[top], arg);
    totalActuals++;
    avm_dec_top();
}
