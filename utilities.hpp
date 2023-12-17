#pragma once

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <string>
#include "token.hpp"

namespace cool {

/*
    There are three kinds of string table entries:
        a true string, a string representation of an identifier
        and a string representation of an integer.

    Having separate tables is convenient for code generation. Different
    data definitions are generated for string constants (StringEntry) and
    integer constants (IntEntry). Identifiers (IdEntry) don't produce
    static data definitions.

    code_def and code_ref are used by the code to produce definitions
    and references (respectively) to constants.
*/

using IdTable = std::unordered_map<Token, std::string>;
using StringTable = std::unordered_map<Token, std::string>;
using IntTable = std::unordered_map<Token, int>;


} // namespace cool