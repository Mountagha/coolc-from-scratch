#pragma once
#include <iostream>
#include "token.hpp"

namespace cool {

static void error(int line, const std::string& msg) {
    std::cerr << "[line: " << line << " ]" << msg << "\n"; 
}

static void fatal_error(int line, const std::string& msg) {
    error(line, msg);
    exit(1);
}

static void report(const Token& t, const std::string& msg) {
    if (t.token_type == EOFILE)
        error(t.loc, " at end " + msg);
    else 
        error(t.loc, " at `" + t.lexeme + "` : " + msg);
}


} // namespace cool