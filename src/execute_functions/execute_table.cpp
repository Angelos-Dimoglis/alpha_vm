#include "../../lib/avm_functions.h"
#include "../../lib/cpu.h"

void execute_newtable(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, nullptr);
    assert(lv && (&stack[0] <= lv && &stack[top] > lv || lv == &retval));

    avm_memcellclear(lv);

    lv->type = table_m;
    lv->data = new avm_table();
    get<avm_table*>(lv->data)->avm_tableincrefcounter();
}