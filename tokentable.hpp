#pragma once

#include "token.hpp"
#include <string>

namespace cool {

/*
    these 3 tables are used to store constants (identifiers,
    string constants and ints)
    This will facilitate code generation.
*/

using IdTable = std::unordered_map<std::string, Token>;
using StringTable = std::unordered_map<std::string, Token>;
using IntTable = std::unordered_map<std::string, Token>;

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

}