#include <assert.h>
#include "../../lib/cpu.h"
#include "../../lib/avm_memcell.h"

extern avm_memcell ax;
extern avm_memcell retval;
extern inline vector<avm_memcell> stack;

extern void avm_warning(string format);
extern void avm_assign(avm_memcell* lv, avm_memcell* rv);

avm_memcell nil_memcell(nil_m);

void execute_assign(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, &nil_memcell);
    avm_memcell* rv = avm_translate_operand(&instr->arg1, &ax);

    assert(lv && (&stack[stack.size()-1] >= lv && lv > &stack.front() || lv == &retval));
    assert(rv && (&stack[stack.size()-1] >= rv && rv > &stack.front() || rv == &retval)); //probably wrong

    avm_assign(lv, rv);
} 
