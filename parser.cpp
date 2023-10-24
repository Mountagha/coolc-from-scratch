#include "parser.hpp"

namespace cool {

Parser::Parser(std::vector<Token>& tokens_): tokens{tokens_}, current{0} hasError{false} {}
Parser::~Parser() = default;
bool Parser::hasError() { return hasError; }

std::unique_ptr<Stmt> Parser::parse() {
    try {
        return parseProgram();
    } catch (ParseError error) {
        synchronize();
        return PStmt{}; // to please to C++ compiler.
    }
}


};