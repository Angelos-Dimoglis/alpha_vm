#pragma once

#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

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

struct avm_memcell_hash {
    size_t operator()(const avm_memcell& m) const;
};

class avm_table {
    private:
        unsigned refCounter;
        unordered_map<avm_memcell, avm_memcell, avm_memcell_hash> indexed;

    public:
        avm_table() : refCounter(0) {}

        void avm_tableincrefcounter();
        void avm_decrefcounter();
        const avm_memcell avm_tablegetelem(const avm_memcell& key) const;
        void avm_tablesetelem(const avm_memcell& key, const avm_memcell& value);
};
