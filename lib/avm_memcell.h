#pragma once

#include <string>
#include <variant>
#include <vector>

class avm_table;

using namespace std;

enum avm_memcell_t {
    number_m    = 0,
    string_m    = 1,
    bool_m      = 2,
    table_m     = 3,
    userfunc_m  = 4,
    libfunc_m   = 5,
    nil_m       = 6,
    undef_m     = 7 
};

struct avm_memcell {
    avm_memcell_t type;
    variant<double, string, bool, avm_table*, unsigned> data;

    bool operator==(const avm_memcell& other) const;

    avm_memcell() : type(undef_m), data(0.0) {}
    avm_memcell(avm_memcell_t type) : type(type), data(0.0) {}
};

inline vector<avm_memcell> stack;
