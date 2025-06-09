#include <cassert>

#include "../lib/rel_table.h"

using namespace std;

void avm_table::avm_tableincrefcounter() {
    refCounter++;
}

void avm_table::avm_decrefcounter() {
    assert(refCounter > 0);
    if (!--refCounter)
        delete this;
}

const avm_memcell avm_table::avm_tablegetelem(const avm_memcell& key) const {
    auto pair = indexed.find(key);

    if (pair != indexed.end())
        return pair->second;

    return nil_memcell;
}

void avm_table::avm_tablesetelem (
    const avm_memcell& key, const avm_memcell& value
) {
    if (value.type == nil_m) {
        indexed.erase(key);
    } else {
        indexed[key] = value;
    }
}
