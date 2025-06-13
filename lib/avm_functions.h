#pragma once
#include <assert.h>
#include "../lib/avm_memcell.h"

typedef void (*memclear_func_t) (avm_memcell*);
typedef bool (*tobool_func_t) (avm_memcell*);
typedef void (*library_func_t) (void);

void memclear_string(avm_memcell* m);
void memclear_table(avm_memcell* m);

extern memclear_func_t memclearFuncs[];

void avm_memcellclear(avm_memcell* m);

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
void avm_dec_top();
void avm_calllibfunc(string funcName);
void avm_callsaveenvironment();
void avm_call_functor(avm_table* t);
void avm_push_table_arg(avm_table* t);
struct userfunc* avm_getfuncinfo(unsigned i);
unsigned avm_get_envvalue(unsigned i);