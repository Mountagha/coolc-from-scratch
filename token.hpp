#pragma once

#include <string>

namespace cool {

enum TokenType {
    // Single character tokens
    LEFT_PAREN, RIGHT_PAREN, COMMA, 
    DOT, MINUS, PLUS, SEMI_COLON, COLON,
    SLASH, STAR, MINUS, PLUS, TILDE, 

    // One or two character token
    EQUAL_EQUAL, ASSIGN,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // literas
    IDENTIFIER, STRING, NUMBER,

    // keywords
    CLASS, ELSE, FALSE, FI, IF, IN, INHERITS, ISVOID, LET,
    LOOP, POOL, THEN, WHILE, CASE, ESAC, NEW, OF, NOT, TRUE,

    EOFILE,

};

class Token {
    public:
        Token() = default;
        Token(TokenType token_type, std::string lexeme, unsigned int loc);

        TokenType token_type;
        std::string lexeme;
        unsigned int loc;
};


} // namespace cool