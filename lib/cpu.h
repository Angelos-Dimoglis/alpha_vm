#include <../lib/avm_instr_set.h>

typedef void (*execute_func_t) (instruction*);

#define AVM_MAX_INSTRUCTIONS (unsigned) nop_v;

extern void execute_assign(instruction*);
extern void execute_add(instruction*);
extern void execute_sub(instruction*);
extern void execute_mul(instruction*);
extern void execute_mod(instruction*);
extern void execute_uminus(instruction*);
extern void execute_and(instruction*);
extern void execute_or(instruction*);
extern void execute_not(instruction*);
extern void execute_jeq(instruction*);
extern void execute_jne(instruction*);
extern void execute_jle(instruction*);
extern void execute_jge(instruction*);
extern void execute_jlt(instruction*);
extern void execute_jgt(instruction*);
extern void execute_call(instruction*);
extern void execute_pusharg(instruction*);
extern void execute_funcenter(instruction*);
extern void execute_funcexit(instruction*);
extern void execute_newtable(instruction*);
extern void execute_tablegetelem(instruction*);
extern void execute_tablesetelem(instruction*);
extern void execute_nop(instruction*);

execute_func_t executeFuncs[] = {
    execute_assign,
    execute_add,
    execute_sub,
    execute_mul,
    execute_mod,
    execute_uminus,
    execute_and,
    execute_or,
    execute_not,
    execute_jeq,
    execute_jne,
    execute_jle,
    execute_jge,
    execute_jlt,
    execute_jgt,
    execute_call,
    execute_pusharg,
    execute_funcenter,
    execute_funcexit,
    execute_newtable,
    execute_tablegetelem,
    execute_tablesetelem,
    execute_nop
};