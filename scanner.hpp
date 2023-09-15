#pragma once

#include <string>
#include <vector>
#include "error.hpp"

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
                // single char token
                case '(': addToken(LEFT_PAREN); break;
                case ')': addToken(RIGHT_PAREN); break;
                case ',': addToken(COMMA); break;
                case '.': addToken(DOT); break;
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

                // Minus and comment
                case '-': {
                    if(match('-')) 
                        comments();
                    else
                        addToken(MINUS); break;
                    break;
                }
                // whitespace
                case '\r':
                case '\t':
                case ' ' :
                case '\0':
                    // skips whitespaces && nul characters
                    break;
                case '\n':
                    line++; break;
                case '"':
                    string();
                    break;
                default:
                    if(isdigit(c))
                        number();
                    else
                        identifierOrKeyword(); 
                

                 
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

        char peek() {
            if (isAtEnd()) return '\0';
            return source[current];
        }

        inline char advance() { return source[current++]; }

        void addToken(TokenType t, std::string lexeme, unsigned int loc) {
            tokens.push_back(Token(t, lexeme, loc));
        }

        void addToken(TokenType t) {
            std::string lexeme = source.substr(start, current-start);
            addToken(t, lexeme, line); 
        }

        void comments() {
            while (!isAtEnd() && peek() != '\n'){
                if (peek() == '\n') line++;
                advance();
            }
        }
        
        void string() {
            while (!isAtEnd() && peek() != '"') {
                advance();
                if(peek() == '\n') line++;
            }
            if (isAtEnd()) {
                error(line, "Unterminated string...");
                return;
            }
            std::string lexeme = source.substr(start+1, current-start+2);
            addToken(STRING, lexeme, line);
        }

        void number() {
            while(isdigit(peek())) {
                advance();
            }
            std::string lexeme = source.substr(start, current-start);
            addToken(NUMBER, lexeme, line);
        }

        void identifierOrKeyword() {
            while(isalnum(peek())) {
                advance();
            }
            std::string lexeme = source.substr(start, current-start);
            addToken(NUMBER, lexeme, line);
        }

};
} //