#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "scanner.hpp"
#include "parser.hpp"
#include "ASTPrinter.hpp"
#include "semant.hpp"
#include "cgen.hpp"
#include "common.hpp"

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
    std::vector<Token> tokens, current_tokens;
    for (int i = 1; i < argc; i++) {
        std::ifstream f{argv[i]};
        if (!f) {
            std::cerr << "failed to open file `" << argv[i] << "`\n";
            exit(EXIT_FAILURE);
        }
        std::stringstream sstr;
        sstr << f.rdbuf();
        std::string current_file_source = sstr.str();
        source += current_file_source;
        
        if (current_file_source.empty()) {
            continue;
        }

        Scanner s{current_file_source};
        current_tokens = s.scanTokens(i == argc-1 ? true : false);
        tokens.insert(tokens.end(), current_tokens.begin(), current_tokens.end());
    }

    std::string out_file = curr_filename.substr(0, curr_filename.find_last_of('.')) + ".s"; 
    std::ofstream out{out_file};
    if (!out.is_open()) {
        std::cerr << "Cannot open `" << out_file << "` for writing.";
        exit(EXIT_FAILURE); // !TODO: better this later.
    }

    if (tokens.empty()) {
        std::cout << "Empty source file(s). " << std::endl;
        exit(EXIT_SUCCESS);
    }

#ifdef DEBUG_PRINT_CODE
    std::cout << "Printing tokens." << std::endl;
    for (auto& token : tokens) {
        std::cout << token << "\n";
    }
#endif
    Parser p{tokens};
    std::cout << "Parsing...\n";
    auto program = p.parse();

    if (p.hasError()) {
        std::cerr << "Compilation halted due to parsing errors." << std::endl;;
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG_PRINT_CODE
    std::cout << "Printing AST...\n";
    ASTPrinter{}.print(program);
#endif
    auto semanter = Semant{};
    std::cout << "Semanting...\n";
    semanter.semant(program);
    if (semanter.hasError()) {
        std::cerr << "Compilation halted due to semantic errors." << std::endl;
        exit(EXIT_FAILURE);
    }
#ifdef DEBUG_PRINT_CODE
    std::cout << "Printing AST after semant analysis..." << std::endl;
    ASTPrinter{}.print(program);
#endif
    std::cout << "Generating code into `" << out_file << "`...\n";
    Cgen{semanter.get_inheritancegraph(), semanter.get_classtable(), out}.cgen(program);
    return 0;   
}