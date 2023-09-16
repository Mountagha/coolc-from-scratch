#include "scanner.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace cool;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage coolc filename.cool\n";
        exit(64);
    }
    std::ifstream f{argv[1]};
    if (!f) {
        std::cerr << "failed to open file" << "\n";
        exit(EXIT_FAILURE);
    }
    std::stringstream sstr;
    sstr << f.rdbuf();
    std::string source = sstr.str();

    Scanner s{source};

    std::vector<Token> tokens;
    tokens = s.scanTokens();
    for (auto& token : tokens) {
        std::cout << token << "\n";
    }

}