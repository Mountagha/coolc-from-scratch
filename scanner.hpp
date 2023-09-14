#pragma once

#include "token.hpp"
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

        void scanToken() {
            char c = advance();
            switch(c) {
                // single char token
                case '(': addToken(LEFT_PAREN); break;
                case ')': addToken(RIGHT_PAREN); break;
                case ',': addToken(COMMA); break;
                case '.': addToken(DOT); break;
                case '-': addToken(MINUS); break;
                case '+': addToken(PLUS); break;
                case ';': addToken(SEMICOLON); break;
                case ':': addToken(COLON); break;
                case '*': addToken(STAR); break;
                case '~': addToken(TILDE); break;
                case '/': addToken(SLASH); break;

                // operators
                case '=': addToken(match('>') ? ARROW : EQUAL); break;
                case '<': addToken(match('-') ? ASSIGN : (match('=') ? LESS_EQUAL : LESS)); break;
                case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break; 
                 
            } 
        }
        bool match(char expected) {
            if (isAtEnd()) return false;
            if (source[current] == expected) {
                current++;
                return true;
            } 
            return false;
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