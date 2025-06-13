#include "../../lib/avm_functions.h"
#include "../../lib/cpu.h"



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
            avm_error("call: cannot bind " + avm_tostring(func) + " to function!");
        }
    }
}