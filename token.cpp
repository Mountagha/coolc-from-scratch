#pragma once 

#include "token.hpp"

namespace cool {

Token::Token(TokenType tok, std::string lex, unsigned int l):
    token_type{tok}, lexeme{lex}, loc{l} {}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << enum_string_map.at(token.token_type) + " " + token.lexeme + "\n";
    return os;
}
};
