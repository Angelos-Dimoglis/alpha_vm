#include <cassert>
#include <iostream>
#include "../lib/avm_memcell.h"
#include "../lib/avm_instr_set.h"

using namespace std;

extern vector<string> namedLibfuncs;
extern vector<userfunc*> userFuncs;

string typeStrings[] = {
    "number",
    "string",
    "bool",
    "table",
    "userfunc",
    "libfunc",
    "nil",
    "undef"
};

bool avm_memcell::operator==(const avm_memcell& other) const {
    return type == other.type && data == other.data;
}

string avm_memcell::tostring() const {
    switch (type) {
        case number_m:
            return to_string(get<double>(data));
        case string_m:
            return "\"" + get<string>(data) + "\"";
        case bool_m:
            return to_string(get<bool>(data));
        case table_m:
            return get<avm_table*>(data)->tostring();
        case userfunc_m:
            return userFuncs[get<unsigned>(data)]->id;
        case libfunc_m:
            return get<string>(data);
        case nil_m:
            return "nil";
        case undef_m:
            cerr << "tostring: cannot convert undefined value!" << endl;
        default:
            assert(0);
    }
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

avm_memcell* avm_table::avm_tablegetelem(const avm_memcell& key) {
    auto pair = indexed.find(key);

    if (pair != indexed.end())
        return &(pair->second);

    return nullptr;
}

void avm_table::avm_tablesetelem (
    const avm_memcell& key, const avm_memcell& value
) {
    if (key.type == undef_m) {
        cerr << "table key cannot be of type 'undef'" << endl;
        assert(0);
    }
    if (value.type == nil_m) {
        indexed.erase(key);
    } else {
        indexed[key] = value;
    }
}

string avm_table::tostring() {
    string s = "";
    for (auto& m : indexed) {

        s += "[ { " + m.first.tostring() + " : " + m.second.tostring() + " }, ";
    }
    s.resize(s.size() - 2);
    s += " ]";
    return s;
}

//TODO: Check of logic is correct
avm_table* avm_table::memberkeys() {
    avm_table* new_table = new avm_table();
    unsigned counter = 0;
    for (auto i : indexed) {
        avm_memcell* new_key = new avm_memcell();
        new_key->type = number_m;
        new_key->data = counter;
        new_table->avm_tablesetelem(*new_key, i.first);
        counter++;
    }
    return new_table;
}

unsigned avm_table::totalmembers() {
    return indexed.size();
}

avm_table* avm_table::copy() {
    avm_table* new_table = new avm_table();
    for (auto i : indexed) {
        new_table->avm_tablesetelem(i.first, i.second);
    }
    return new_table;
}

void memclear_string(avm_memcell* m) {
    assert(m->data.index() == 1);  // ensure it is a string
    get<string>(m->data).clear();                // clear the contents
}

void memclear_table(avm_memcell* m) {
    assert(m->data.index() == 3);
    get<avm_table*>(m->data)->avm_decrefcounter();
}

memclear_func_t memclearFuncs[] = {
    0, /*number*/
    memclear_string,
    0, /*bool*/
    memclear_table,
    0, /*usefunc*/
    0, /*libfunc*/
    0, /*nil*/
    0  /*undef*/
};

void avm_memcellclear(avm_memcell* m) {
    if (m->type != undef_m) {
        memclear_func_t f = memclearFuncs[m->type];
        if (f) {
            (*f) (m);
        }
        m->type = undef_m;
    }
}
