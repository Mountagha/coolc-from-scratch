#include "parser.hpp"

namespace cool {

Parser::Parser(std::vector<Token>& tokens_): tokens{tokens_}, current{0} {}

Parser::~Parser() = default;

std::unique_ptr<Stmt> Parser::parse() {
    try {
        return parseProgram();
    } catch (ParseError error) {
        synchronize();
    }
}


};