#pragma once
#include <iostream>

namespace cool {

static void error(int line, const std::string& msg) {
    std::cerr << "[line: " << line << " ]" << msg << "\n"; 
}

static void fatal_error(int line, const std::string& msg) {
    error(line, msg);
    exit(1);
}


} // namespace cool