#include <assert.h>
#include "../../lib/cpu.h"
#include "../../lib/avm_functions.h"

void execute_assign(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, &nil_memcell);
    avm_memcell* rv = avm_translate_operand(&instr->arg1, &ax);

    assert(lv && (&stack[stack.size()-1] >= lv && lv > &stack.front() || lv == &retval));
    assert(rv); //probably wrong

    avm_assign(lv, rv);
} 
