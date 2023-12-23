#pragma once

#include <string>
#include <unordered_map>

namespace cool {

enum TokenType {
    // Single character tokens
    LEFT_PAREN, RIGHT_PAREN, 
    LEFT_BRACE, RIGHT_BRACE, COMMA, 
    DOT, MINUS, PLUS, SEMICOLON, COLON,
    SLASH, STAR, TILDE, AT, 

    // One or two character token
    EQUAL, ASSIGN,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL, ARROW,

    // literals
    IDENTIFIER, STRING, NUMBER,

    // keywords
    CLASS, ELSE, FALSE, FI, IF, IN, INHERITS, ISVOID, LET,
    LOOP, POOL, THEN, WHILE, CASE, ESAC, NEW, OF, NOT, TRUE,

    EOFILE,

};

const std::unordered_map<int, std::string> enum_string_map = {
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::LEFT_BRACE, "LEFT_BRACE"},
    {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::DOT, "DOT"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::COLON, "COLON"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::STAR, "STAR"},
    {TokenType::TILDE, "TILDE"},
    {TokenType::AT, "@"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::ARROW, "ARROW"},
    {TokenType::ASSIGN, "ASSIGN"},
    {TokenType::GREATER, "GREATER"},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenType::LESS, "LESS"},
    {TokenType::LESS_EQUAL, "LESS_EQUAL"},
    {TokenType::CLASS, "CLASS"},
    {TokenType::ELSE, "ELSE"},
    {TokenType::FALSE, "FALSE"},
    {TokenType::IF, "IF"},
    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::STRING, "STRING"},
    {TokenType::NUMBER, "NUMBER"},
    {TokenType::FI, "FI"},
    {TokenType::IN, "IN"},
    {TokenType::INHERITS, "INHERITS"},
    {TokenType::ISVOID, "ISVOID"},
    {TokenType::LET, "LET"},
    {TokenType::LOOP, "LOOP"},
    {TokenType::POOL, "POOL"},
    {TokenType::WHILE, "WHILE"},
    {TokenType::CASE, "CASE"},
    {TokenType::ESAC, "ESAC"},
    {TokenType::NEW, "NEW"},
    {TokenType::OF, "OF"},
    {TokenType::NOT, "NOT"},
    {TokenType::TRUE, "TRUE"},
    {TokenType::EOFILE, "EOFILE"},
};

class Token {
    public:
        Token() = default;
        Token(TokenType token_type, std::string lexeme, unsigned int loc=0);
        friend std::ostream& operator<<(std::ostream& os, const Token& token);
        friend bool operator==(const Token& a, const Token& b);
        friend bool operator!=(const Token& a, const Token& b);
        friend bool operator<(const Token& a, const Token& b);
    
        TokenType token_type;
        std::string lexeme;
        unsigned int loc;
};


} // namespace cool