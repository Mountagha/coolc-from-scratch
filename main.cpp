#include <iostream>
#include <fstream>
#include <sstream>

#include "scanner.hpp"
#include "parser.hpp"
#include "ASTPrinter.hpp"
#include "semant.hpp"

using namespace cool;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage coolc [filename.cool...]\n";
        exit(64);
    }
    std::string source;
    for (int i = 1; i < argc; i++) {
        std::ifstream f{argv[i]};
        if (!f) {
            std::cerr << "failed to open file `" << argv[i] << "`\n";
            exit(EXIT_FAILURE);
        }
        std::stringstream sstr;
        sstr << f.rdbuf();
        std::string file_source = sstr.str();
        source += file_source;
    }

    Scanner s{source};

    std::vector<Token> tokens;
    tokens = s.scanTokens();
    for (auto& token : tokens) {
        std::cout << token << "\n";
    }
    Parser p{tokens};
    std::cout << "Parsing...\n";
    auto program = p.parse();

    if(!p.hasError()) {
        std::cout << "Printing AST...\n";
        ASTPrinter{}.print(program);
        std::cout << "Semanting...\n";
        Semant{}.semant(program);
        std::cout << "Printing AST after semant analysis...\n";
        ASTPrinter{}.print(program);

    }
        
}