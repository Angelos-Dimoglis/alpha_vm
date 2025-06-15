#include "../../lib/avm_functions.h"
#include "../../lib/cpu.h"

void execute_newtable(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    assert(lv && (&stack[AVM_STACK_SIZE-1] >= lv && &stack[top] < lv || lv == &retval));

    avm_memcellclear(lv);

    lv->type = table_m;
    lv->data = new avm_table();
    get<avm_table*>(lv->data)->avm_tableincrefcounter();
}

void execute_tablegetelem(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    avm_memcell* t = avm_translate_operand(&instr->arg1, (avm_memcell*) 0);
    avm_memcell* i = avm_translate_operand(&instr->arg2, &ax);

    assert(lv && (&stack[AVM_STACK_SIZE-1] >= lv && &stack[top] < lv || lv == &retval));
    assert(t && &stack[AVM_STACK_SIZE-1] >= t && &stack[top] < t);
    assert(i);

    avm_memcellclear(lv);
    lv->type = nil_m;

    if (t->type != table_m) {
        avm_error("illegal use of type " + typeStrings[t->type] + " as table!");
    }
    else {
        avm_memcell* content = get<avm_table*>(t->data)->avm_tablegetelem(*i);
        if (content) {
            avm_assign(lv, content);
        }
        else {
            avm_warning(t->tostring() + "[" + i->tostring() + "] not found!");
        }
    }
}

void execute_tablesetelem(instruction* instr) {
    avm_memcell* t = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    avm_memcell* i = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* c = avm_translate_operand(&instr->arg2, &bx);

    assert(t && &stack[AVM_STACK_SIZE-1] >= t && &stack[top] < t);
    assert(i && c);

    if (t->type != table_m) {
        avm_error("illegal use of type " + typeStrings[t->type] + " as table!");
    }
    else {
        get<avm_table*>(t->data)->avm_tablesetelem(*i, *c);
    }
}
