#pragma once

#include <string>
#include <unordered_map>

namespace cool {

enum TokenType {
    // Single character tokens
    LEFT_PAREN, RIGHT_PAREN, COMMA, 
    DOT, MINUS, PLUS, SEMI_COLON, COLON,
    SLASH, STAR, TILDE, 

    // One or two character token
    EQUAL_EQUAL, ASSIGN,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // literals
    IDENTIFIER, STRING, NUMBER,

    // keywords
    CLASS, ELSE, FALSE, FI, IF, IN, INHERITS, ISVOID, LET,
    LOOP, POOL, THEN, WHILE, CASE, ESAC, NEW, OF, NOT, TRUE,

    EOFILE,

};

const std::unordered_map<int, std::string> enum_string_map = {
    {TokenType::LEFT_PAREN, "("},
    {TokenType::RIGHT_PAREN, ")"},
    {TokenType::COMMA, ","},
    {TokenType::DOT, "."},
    {TokenType::MINUS, "-"},
    {TokenType::PLUS, "+"},
    {TokenType::SEMI_COLON, ";"},
    {TokenType::COLON, ":"},
    {TokenType::SLASH, "/"},
    {TokenType::STAR, "*"},
    {TokenType::TILDE, "~"},
    {TokenType::EQUAL_EQUAL, "=="},
    {TokenType::ASSIGN, "<-"},
    {TokenType::GREATER, ">"},
    {TokenType::GREATER_EQUAL, ">="},
    {TokenType::LESS, "<"},
    {TokenType::LESS_EQUAL, "<="},
    {TokenType::CLASS, "class"},
    {TokenType::ELSE, "else"},
    {TokenType::FALSE, "false"},
    {TokenType::IF, "if"},
    {TokenType::FI, "fi"},
    {TokenType::IN, "in"},
    {TokenType::INHERITS, "inherits"},
    {TokenType::ISVOID, "isvoid"},
    {TokenType::LET, "let"},
    {TokenType::LOOP, "loop"},
    {TokenType::POOL, "pool"},
    {TokenType::THEN, "then"},
    {TokenType::WHILE, "while"},
    {TokenType::CASE, "case"},
    {TokenType::ESAC, "esac"},
    {TokenType::NEW, "new"},
    {TokenType::OF, "of"},
    {TokenType::NOT, "not"},
    {TokenType::TRUE, "true"},
};

class Token {
    public:
        Token() = default;
        Token(TokenType token_type, std::string lexeme, unsigned int loc);
        friend std::ostream& operator<<(std::ostream& os, const Token& token);
    
        TokenType token_type;
        std::string lexeme;
        unsigned int loc;
};


} // namespace cool