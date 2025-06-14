#include <assert.h>
#include "../../lib/cpu.h"
#include "../../lib/avm_functions.h"

void execute_assign(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    avm_memcell* rv = avm_translate_operand(&instr->arg1, &ax);

    assert(lv && (&stack[0] <= lv && &stack[top] > lv || lv == &retval));
    assert(rv); //probably wrong

    avm_assign(lv, rv);
} 

void execute_nop(instruction*){}