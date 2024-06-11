#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "scanner.hpp"
#include "parser.hpp"
#include "ASTPrinter.hpp"
#include "semant.hpp"
#include "cgen.hpp"

using namespace cool;

// Used by the error handling in both lexer and parser and the
// codegen as well.
std::string cool::curr_filename;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage coolc [filename.cool...]\n";
        exit(64);
    }
    std::string source;
    curr_filename = std::string(argv[1]);   // handle later for multiple files.
    curr_filename = curr_filename.substr(curr_filename.find_last_of('/') + 1);
    std::cout << curr_filename << std::endl;
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

    std::string out_file = curr_filename.substr(0, curr_filename.find_last_of('.')) + ".s"; 
    std::ofstream out{out_file};
    if (!out.is_open()) {
        std::cerr << "Cannot open `" << out_file << "` for writing.";
        exit(EXIT_FAILURE); // !TODO: better this later.
    }
    
    Scanner s{source};

    std::vector<Token> tokens;
    tokens = s.scanTokens();
    //for (auto& token : tokens) {
    //    std::cout << token << "\n";
    //}
    Parser p{tokens};
    std::cout << "Parsing...\n";
    auto program = p.parse();

    if(!p.hasError()) {
        //std::cout << "Printing AST...\n";
        //ASTPrinter{}.print(program);
        auto semanter = Semant{};
        std::cout << "Semanting...\n";
        semanter.semant(program);
        if (!semanter.hasError()) {
            std::cout << "Printing AST after semant analysis...\n";
            ASTPrinter{}.print(program);
            Cgen{semanter.get_inheritancegraph(), semanter.get_classtable(), out}.cgen(program);
        }
    }
        
}