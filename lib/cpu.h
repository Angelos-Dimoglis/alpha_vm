#pragma once

#include "../lib/avm_instr_set.h"
#include "../lib/avm_memcell.h"

#define AVM_STACKENV_SIZE 4

typedef void (*execute_func_t) (instruction*);

extern vector<avm_memcell> stack;
extern avm_memcell ax, bx, cx, retval;
extern unsigned top, topsp;

extern bool executionFinished;
extern unsigned pc;
extern unsigned codeSize;
extern struct instruction *code;

#define AVM_ENDING_PC codeSize

#define AVM_MAX_INSTRUCTIONS (unsigned) nop_v

#define execute_add execute_arithmetic
#define execute_sub execute_arithmetic
#define execute_mul execute_arithmetic
#define execute_div execute_arithmetic
#define execute_mod execute_arithmetic

#define execute_jle execute_comparison
#define execute_jge execute_comparison
#define execute_jlt execute_comparison
#define execute_jgt execute_comparison

extern void execute_assign(instruction*);
extern void execute_add(instruction*);
extern void execute_sub(instruction*);
extern void execute_mul(instruction*);
extern void execute_mod(instruction*);
extern void execute_jeq(instruction*);
extern void execute_jne(instruction*);
extern void execute_jle(instruction*);
extern void execute_jge(instruction*);
extern void execute_jlt(instruction*);
extern void execute_jgt(instruction*);
extern void execute_jmp(instruction*);
extern void execute_call(instruction*);
extern void execute_pusharg(instruction*);
extern void execute_funcenter(instruction*);
extern void execute_funcexit(instruction*);
extern void execute_newtable(instruction*);
extern void execute_tablegetelem(instruction*);
extern void execute_tablesetelem(instruction*);
extern void execute_nop(instruction*);

avm_memcell* avm_translate_operand (vmarg* arg, avm_memcell* reg);

void execute_cycle (void);
