#include <cassert>
#include "../lib/avm_memcell.h"

using namespace std;

avm_memcell nil_memcell(nil_m);

bool avm_memcell::operator==(const avm_memcell& other) const {
    return type == other.type && data == other.data;
}

size_t avm_memcell_hash::operator()(const avm_memcell& m) const {
    size_t hashValue = hash<int>()(static_cast<int>(m.type));

    if (holds_alternative<double>(m.data)) {
        hashValue ^= hash<double>()(get<double>(m.data));
    }
    else if (holds_alternative<string>(m.data)) {
        hashValue ^= hash<string>()(get<string>(m.data));
    }
    else if (holds_alternative<bool>(m.data)) {
        hashValue ^= hash<bool>()(get<bool>(m.data));
    }
    else if (holds_alternative<avm_table*>(m.data)) {
        hashValue ^= hash<avm_table*>()(get<avm_table*>(m.data));
    }
    else if (holds_alternative<unsigned>(m.data)) {
        hashValue ^= hash<unsigned>()(get<unsigned>(m.data));
    }
    return hashValue;
}

// ### AVM TABLE ###

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
