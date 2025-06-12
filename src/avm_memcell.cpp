#include "../lib/avm_memcell.h"

bool avm_memcell::operator==(const avm_memcell& other) const {
        return type == other.type && data == other.data;
}