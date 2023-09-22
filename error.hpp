#pragma once
#include <iostream>

namespace cool {

static void error(int line, const std::string& msg) {
    std::cerr << "[line: " << line << " ]" << msg << "\n"; 
}

} // namespace cool