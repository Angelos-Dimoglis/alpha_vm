#pragma once
#include <assert.h>
#include "../lib/avm_memcell.h"
#include "../lib/avm_instr_set.h"

#define AVM_NUMACTUALS_OFFSET   4
#define AVM_SAVEDPC_OFFSET      3
#define AVM_SAVEDTOP_OFFSET     2
#define AVM_SAVEDTOPSP_OFFSET   1

typedef bool (*tobool_func_t) (avm_memcell*);
typedef void (*library_func_t) (void);

void avm_warning(string format);

void avm_assign(avm_memcell* lv, avm_memcell* rv);

void avm_error(string format);

bool number_tobool(avm_memcell* m);
bool string_tobool(avm_memcell* m);
bool bool_tobool(avm_memcell* m);
bool table_tobool(avm_memcell* m);
bool userfunc_tobool(avm_memcell* m);
bool libfunc_tobool(avm_memcell* m);
bool nil_tobool(avm_memcell* m);
bool undef_tobool(avm_memcell* m);

extern tobool_func_t toboolFuncs[];

bool avm_tobool(avm_memcell* m);

string avm_tostring(avm_memcell*);
void avm_inc_top();
void avm_calllibfunc(string funcName);
void avm_callsaveenvironment();
void avm_call_functor(avm_table* t);
void avm_push_table_arg(avm_table* t);
userfunc* avm_getfuncinfo(unsigned i);
unsigned avm_get_envvalue(unsigned i);
unsigned avm_totalactuals();
avm_memcell* avm_getactual(unsigned i);
void avm_registerlibfunc(string id, library_func_t addr);

extern void libfunc_print();
extern void libfunc_input();
extern void libfunc_objectmemberkeys();
extern void libfunc_objecttotalmembers();
extern void libfunc_objectcopy();
extern void libfunc_totalarguments();
extern void libfunc_argument();
extern void libfunc_typeof();
extern void libfunc_strtnum();
extern void libfunc_sqrt();
extern void libfunc_cos();
extern void libfunc_sin();

extern library_func_t libFuncs[];