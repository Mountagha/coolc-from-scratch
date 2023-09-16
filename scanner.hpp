#pragma once

#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <array>
#include "error.hpp"
#include "token.hpp"

namespace cool {

const std::array<std::string, 18> keywords = {
    "class",
    "else",
    "false",
    "if",
    "fi",
    "in",
    "inherits",
    "isvoid",
    "let",
    "loop",
    "pool",
    "while",
    "case",
    "esac",
    "new",
    "of",
    "not",
    "true",
};


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
                    if(isDigit(c))
                        number();
                    else if (isAlphanumeric(c))
                        identifierOrKeyword(); 
                    break;
                 
            } 
        }

        inline bool isDigit(char c) {
            return '0' <= c && c <= '9';
        }

        inline bool isAlpha(char c) {
            return (c >= 'a' && c <= 'z' ||
                    c >= 'A' && c <= 'Z' ||
                    c == '_');
        }
        
        inline bool isAlphanumeric(char c) {
            return isDigit(c) || isAlpha(c);
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
                if(peek() == '\n') line++;
                advance();
            }
            if (isAtEnd()) {
                error(line, "Unterminated string...");
                return;
            }
            // consume the closing "
            advance();
            std::string lexeme = source.substr(start+1, current-start-2);
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
            while(isAlphanumeric(peek())) {
                advance();
            }
            std::string lexeme = source.substr(start, current-start);
            if (isKeyword(lexeme))
                addToken(IDENTIFIER, lexeme, line);
            else
                addToken(IDENTIFIER, lexeme, line);
        }

        std::string strTolower(const std::string& s) {
            std::string lowerStr;
            std::transform(s.begin(), s.end(), lowerStr.begin(),
                [](unsigned char c) {return std::tolower(c); });
            return lowerStr;
        }

        bool isKeyword(const std::string& s) {
            if(std::find(keywords.begin(), keywords.end(), s) != keywords.end())
                return true;
            return false;
        }

};
} //