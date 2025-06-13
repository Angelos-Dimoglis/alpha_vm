#include <cassert>

#include "../lib/avm_functions.h"
#include "../lib/cpu.h"

#define execute_add execute_arithmetic
#define execute_sub execute_arithmetic
#define execute_mul execute_arithmetic
#define execute_div execute_arithmetic
#define execute_mod execute_arithmetic

#define execute_jle execute_comparison
#define execute_jge execute_comparison
#define execute_jlt execute_comparison
#define execute_jgt execute_comparison

extern avm_memcell ax, bx, retval;
extern unsigned pc;
extern bool executionFinished;

extern vector<avm_memcell> stack;

avm_memcell nil_memcell(nil_m);

typedef double (*arithmetic_func_t) (double x, double y);

double add_impl(double x, double y) { return x + y; }
double sub_impl(double x, double y) { return x - y; }
double mul_impl(double x, double y) { return x * y; }
double div_impl(double x, double y) {
    if (y == 0.0) {
        avm_warning("Cannot divide with 0!");
        assert(0);
    } else {
        return x / y;
    }
}

double mod_impl(double x, double y) {
    if (((unsigned) y) == 0) {
        avm_warning("Cannot modulo with 0!");
        assert(0);
    } else {
        return ((unsigned) x) % ((unsigned) y);
    }
}

arithmetic_func_t arithmeticFuncs[] = {
    add_impl,
    sub_impl,
    mul_impl,
    div_impl,
    mod_impl
};

void execute_arithmetic (instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, &nil_memcell);
    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    assert(lv && (&stack[0] <= lv && &stack.front() > lv || lv == &retval));
    assert(rv1 && rv2);
    if (rv1->type != number_m || rv2->type != number_m) {
        avm_error("Not a number in arithmetic!");
    } else {
        arithmetic_func_t op = arithmeticFuncs[instr->opcode - add_v];
        avm_memcellclear(lv);
        lv->type = number_m;
        lv->data = (*op) (get<double>(rv1->data), get<double>(rv2->data));
    }
}

void execute_assign(instruction* instr) {
    avm_memcell* lv = avm_translate_operand(&instr->result, &nil_memcell);
    avm_memcell* rv = avm_translate_operand(&instr->arg1, &ax);

    assert(lv && (&stack[stack.size()-1] >= lv && lv > &stack.front() || lv == &retval));
    assert(rv && (&stack[stack.size()-1] >= rv && rv > &stack.front() || rv == &retval)); //probably wrong

    avm_assign(lv, rv);
} 

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

typedef bool (*cmp_func) (double, double);

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
