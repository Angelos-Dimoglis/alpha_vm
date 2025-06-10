#pragma once

#include <unordered_map>

#include "avm_memcell.h"

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
