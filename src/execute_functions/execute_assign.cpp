#include <assert.h>
#include "../../lib/cpu.h"
#include "../../lib/avm_functions.h"

void execute_assign(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, nullptr);
    avm_memcell* rv = avm_translate_operand(&instr->arg1, &ax);

    assert(lv && (&stack[0] >= lv && lv > &stack.front() || lv == &retval));
    assert(rv); //probably wrong

    avm_assign(lv, rv);
} 
