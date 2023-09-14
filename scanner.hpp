#pragma once

#include <string>
#include <vector>

namespace cool {


class Scanner {
    public:
        Scanner(const std::string& source);
        std::vector<Token> scanTokens();
    private:
        unsigned int start;
        unsigned int current;
        unsigned int line;
        const std::string& source;
        std::vector<Token> tokens;

        inline bool const isAtEnd() { return current > source.length(); }

        std::vector<Token> scanToken() {
            char c = advance();
            switch(c) {
                case '(': addToken(LEFT_PAREN); break;
                case ')': addToken(RIGHT_PAREN); break;
                case ',': addToken(COMMA); break;
                case '.': addToken(DOT); break;
                case '-': addToken(MINUS); break;
                case '+': addToken(PLUS); break;
                case ';': addToken(SEMICOLON); break;
                 
            } 
        }

        inline char advance() { return source[current++]; }

        void addToken(TokenType t, std::string lexeme, unsigned int loc) {
            tokens.push_back(Token(t, lexeme, loc));
        }

        void addToken(TokenType t) {
            std::string lexeme = source.substr(start, current-start);
            addToken(t, lexeme, line); 
        }

};
} //