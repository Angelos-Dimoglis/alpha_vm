#include "../../lib/avm_functions.h"
#include "../../lib/cpu.h"

typedef double (*arithmetic_func_t) (double x, double y);

double add_impl(double x, double y) { return x + y; }
double sub_impl(double x, double y) { return x - y; }
double mul_impl(double x, double y) { return x * y; }
double div_impl(double x, double y) {
    if (y == 0.0) {
        avm_error("Cannot divide with 0!");
        assert(0);
    } else {
        return x / y;
    }
}

double mod_impl(double x, double y) {
    if (((unsigned) y) == 0) {
        avm_error("Cannot modulo with 0!");
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
    avm_memcell* lv = avm_translate_operand(&instr->result, (avm_memcell*) 0);
    avm_memcell* rv1 = avm_translate_operand(&instr->arg1, &ax);
    avm_memcell* rv2 = avm_translate_operand(&instr->arg2, &bx);

    assert(lv && (&stack[AVM_STACK_SIZE-1] >= lv && &stack[top] > lv || lv == &retval));
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
