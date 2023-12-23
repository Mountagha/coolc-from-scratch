#include "token.hpp"

namespace cool {

Token::Token(TokenType tok, std::string lex, unsigned int l):
    token_type{tok}, lexeme{lex}, loc{l} {}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    if (enum_string_map.find(token.token_type) != enum_string_map.end())
        os << "[line " + std::to_string(token.loc) + " ] " + 
            enum_string_map.at(token.token_type) + " " 
            + token.lexeme + "\n";
    else
        os << "[line " + std::to_string(token.loc) +  " ] UNKNOWN " + token.lexeme + "\n";
    return os;
}

bool operator==(const Token& a, const Token& b) {
    if (a.token_type != b.token_type) return false;
    return a.lexeme == b.lexeme;
}

bool operator!=(const Token& a, const Token& b) {
    return !(a == b);
}

bool operator<(const Token& a, const Token& b) {
    if (a.token_type == b.token_type) 
        return a.lexeme < b.lexeme;
    return a.token_type < b.token_type;
}
};
