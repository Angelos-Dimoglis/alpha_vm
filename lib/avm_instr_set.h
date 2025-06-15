#pragma once

#include <string>

using namespace std;

enum vmopcode {
    assign_v       = 0,
    add_v          = 1,
    sub_v          = 2,
    mul_v          = 3,
    div_v          = 4,
    mod_v          = 5,
    jump_v         = 6,
    jeq_v          = 7,
    jne_v          = 8,
    jle_v          = 9,
    jge_v          = 10,
    jlt_v          = 11,
    jgt_v          = 12,
    call_v         = 13,
    pusharg_v      = 14,
    funcenter_v    = 15,
    funcexit_v     = 16,
    newtable_v     = 17,
    tablegetelem_v = 18,
    tablesetelem_v = 19,
    nop_v          = 20
};

enum vmarg_t {
    label_a    = 0,
    global_a   = 1,
    formal_a   = 2,
    local_a    = 3,
    number_a   = 4,
    string_a   = 5,
    bool_a     = 6,
    nil_a      = 7,
    userfunc_a = 8,
    libfunc_a  = 9,
    retval_a   = 10
};

struct vmarg {
    vmarg_t type;
    unsigned val;
};

struct instruction {
    vmopcode opcode;
    vmarg result;
    vmarg arg1;
    vmarg arg2;
    unsigned srcLine;
};

struct userfunc {
    unsigned address;
    unsigned localSize;
    string id;

    userfunc() : address(-1), localSize(-1), id("") {}

    userfunc(unsigned address, unsigned localSize, string id) : address(address), localSize(localSize), id(id) {}
};
