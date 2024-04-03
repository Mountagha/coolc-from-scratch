#pragma once

#include <string>
#include <utility>
#include <map>

#include "token.hpp"

namespace cool {

/*
    these 3 tables are used to store constants (identifiers,
    string constants and ints)
    This will facilitate code generation.
*/

class TokenTable {
    private:
        std::map<std::string, Token> table_elements{};
        std::map<std::string, size_t> table_idx{}; 
        int idx;
    public:
        TokenTable();
        void insert(std::string id, Token token);
        int get_index(const std::string& id);
        std::map<std::string, Token>& get_elements();
};

class IdTable : public TokenTable {};
class StringTable : public TokenTable {};
class IntTable : public TokenTable {};

inline IdTable& idtable() {
    static IdTable idtable;
    return idtable;
}

inline StringTable& stringtable() {
    static StringTable stringtable;
    return stringtable;
}

inline IntTable& inttable() {
    static IntTable inttable;
    return inttable;
}

}; // namespace cool