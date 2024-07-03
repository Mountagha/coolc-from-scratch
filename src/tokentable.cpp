#include "tokentable.hpp"

namespace cool {

TokenTable::TokenTable(): idx{0} {}

void TokenTable::insert(std::string id, Token token) {
    if (table_idx.find(id) != table_idx.end()) {
        return;
    }
    table_elements.insert({id, token});
    table_idx.insert({id, idx});
    idx++;
}

int TokenTable::get_index(const std::string& id) {
    if (table_idx.find(id) != table_idx.end()) {
        return table_idx.at(id);
    }
    return -1;
}

std::map<std::string, Token>& TokenTable::get_elements() {
    return table_elements;
}

}; // namespace cool