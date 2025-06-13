#include "../../lib/avm_functions.h"
#include "../../lib/cpu.h"

typedef bool (*cmp_func) (double, double);

extern bool executionFinished;
extern unsigned pc;

tobool_func_t toboolFuncs[] = {
    number_tobool,
    string_tobool,
    bool_tobool,
    table_tobool,
    userfunc_tobool,
    libfunc_tobool,
    nil_tobool,
    undef_tobool
};

string typeStrings[] = {
    "number",
    "string",
    "bool",
    "table",
    "userfunc",
    "libfunc",
    "nil",
    "undef"
};

bool jle_impl(double x, double y) { return x <= y; }
bool jge_impl(double x, double y) { return x >= y; }
bool jlt_impl(double x, double y) { return x < y; }
bool jgt_impl(double x, double y) { return x > y; }

cmp_func comparisonFuncs[] = {
    jle_impl,
    jge_impl,
    jlt_impl,
    jgt_impl
};

void execute_jeq(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    bool result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m) {
        avm_error("'undef' involved in equality!");
    }
    else if (rv1->type == nil_m || rv2->type == nil_m) {
        result = rv1->type == nil_m && rv2->type == nil_m;
    }
    else if (rv1->type == bool_m || rv2->type == bool_m) {
        result = (avm_tobool(rv1) == avm_tobool(rv2));
    }
    else if (rv1->type != rv2->type) {
        avm_error(typeStrings[rv1->type] + " == " + typeStrings[rv2->type] + " is illegal!");
    }
    else {
        tobool_func_t op = toboolFuncs[rv1->type];
        result = (*op) (rv1) == (*op) (rv2);
    }

    if (!executionFinished && result) {
        pc = instr->result.val;
    }
}

void execute_jne(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    bool result = 0;

    if (rv1->type == undef_m || rv2->type == undef_m) {
        avm_error("'undef' involved in unequality!");
    }
    else if (rv1->type == nil_m || rv2->type == nil_m) {
        result = rv1->type != nil_m || rv2->type != nil_m;
    }
    else if (rv1->type == bool_m || rv2->type == bool_m) {
        result = (avm_tobool(rv1) != avm_tobool(rv2));
    }
    else if (rv1->type != rv2->type) {
        avm_error(typeStrings[rv1->type] + " != " + typeStrings[rv2->type] + " is illegal!");
    }
    else {
        tobool_func_t op = toboolFuncs[rv1->type];
        result = (*op) (rv1) != (*op) (rv2);
    }
    
    if (!executionFinished && result) {
        pc = instr->result.val;
    }
}

void execute_comparison(instruction* instr) {
    assert(instr->result.type == label_a);

    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);
    
    assert(rv1 && rv2);

    bool result = 0;

    if (rv1->type != number_m || rv2->type != number_m) {
        avm_error("Not a number in comparison!");
    }
    else {
        cmp_func op = comparisonFuncs[instr->opcode - jle_v];
        result = (*op) (get<double>(rv1->data), get<double>(rv2->data));
    }

    if (!executionFinished && result) {
        pc = instr->result.val;
    }
}
