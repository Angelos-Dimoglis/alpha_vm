#pragma once

#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

using namespace std;

extern string typeStrings[];

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

class avm_table; // forward declare

struct avm_memcell {
    avm_memcell_t type;
    variant<double, string, bool, avm_table*, unsigned> data;

    bool operator==(const avm_memcell& other) const;

    avm_memcell() : type(undef_m), data(0.0) {}

    string tostring() const;
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
        avm_memcell* avm_tablegetelem(const avm_memcell& key);
        void avm_tablesetelem(const avm_memcell& key, const avm_memcell& value);
        string tostring();
        avm_table* memberkeys();
        unsigned totalmembers();
        avm_table* copy();
};

typedef void (*memclear_func_t) (avm_memcell*);

void memclear_string(avm_memcell* m);
void memclear_table(avm_memcell* m);

extern memclear_func_t memclearFuncs[];

void avm_memcellclear(avm_memcell* m);
