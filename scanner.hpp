#pragma once

#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <algorithm>
#include "error.hpp"
#include "token.hpp"

namespace cool {

static const std::unordered_map<std::string, TokenType> keywordsMap = {
    {"class", CLASS},
    {"else", ELSE},
    {"false", FALSE},
    {"if", IF},
    {"then", THEN},
    {"fi", FI},
    {"in", IN},
    {"inherits", INHERITS},
    {"isvoid", ISVOID},
    {"let", LET},
    {"loop", LOOP},
    {"pool", POOL},
    {"while", WHILE},
    {"case", CASE},
    {"esac", ESAC},
    {"new", NEW},
    {"of", OF},
    {"not", NOT},
    {"true", TRUE},
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
                case ')': addToken(RIGHT_PAREN); break;
                case '}': addToken(RIGHT_BRACE); break;
                case '{': addToken(LEFT_BRACE); break;
                case ',': addToken(COMMA); break;
                case '.': addToken(DOT); break;
                case '+': addToken(PLUS); break;
                case ';': addToken(SEMICOLON); break;
                case ':': addToken(COLON); break;
                case '*': addToken(STAR); break;
                case '~': addToken(TILDE); break;
                case '/': addToken(SLASH); break;
                case '@': addToken(AT); break;

                // operators
                case '=': addToken(match('>') ? ARROW : EQUAL); break;
                case '<': addToken(match('-') ? ASSIGN : (match('=') ? LESS_EQUAL : LESS)); break;
                case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break; 

                // left parenthese or long comment.
                case '(': { 
                    if (match('*'))
                        long_comment();
                    else
                        addToken(LEFT_PAREN); 
                    break;
                }

                // Minus or short comment
                case '-': {
                    if(match('-')) 
                        short_comment();
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

        char previous() {
            return current>0 ? source[current] : source[0];
        }

        inline char advance() { return source[current++]; }

        void addToken(TokenType t, std::string lexeme, unsigned int loc) {
            tokens.push_back(Token(t, lexeme, loc));
        }

        void addToken(TokenType t) {
            std::string lexeme = source.substr(start, current-start);
            addToken(t, lexeme, line); 
        }

        void short_comment() {
            while (!isAtEnd() && peek() != '\n'){
                if (peek() == '\n') line++;
                advance();
            }
        }

        void long_comment() {
            bool isBalanced = false;
            std::stack<int> nesting{};
            nesting.push(1);
            while (!isBalanced && !isAtEnd()){
                if (match('(') && match('*')) 
                    nesting.push(1);
                else if(match('*') && match(')')) 
                    nesting.pop();
                if (nesting.empty()) 
                    isBalanced = true;
                if (peek() == '\n') line++;
                advance();
            }
            if (!isBalanced)
                error(line, "Unterminated long comments.");
        }
        
        void string() {
            while (!isAtEnd() && (peek() != '"' && previous() != '\\')) {
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
                addToken(keywordsMap.at(strTolower(lexeme)), lexeme, line);
            else
                addToken(IDENTIFIER, lexeme, line);
        }

        std::string strTolower(const std::string& s) {
            std::string lowerStr = s;
            std::transform(lowerStr.cbegin(), lowerStr.cend(), lowerStr.begin(),
                [](unsigned char c) {return std::tolower(c); });
            return lowerStr;
        }

        bool isKeyword(const std::string& s) {
            if (keywordsMap.find(strTolower(s)) != keywordsMap.end())
                return true;
            return false;
        }

};
} //