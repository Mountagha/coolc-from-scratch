#include <iostream>

namespace cool {

void error(int line, const std::string& msg) {
    std::cerr << "[line: " << line << " ]" << msg << "\n"; 
}
} // namespace cool