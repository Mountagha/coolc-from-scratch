#pragma once

#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <algorithm>
#include "error.hpp"
#include "token.hpp"
#include "tokentable.hpp"

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

extern std::string curr_filename;


class Scanner {
    public:
        Scanner(const std::string& source);
        std::vector<Token> scanTokens(bool last_file=false);
    private:
        unsigned int start;
        unsigned int current;
        unsigned int line;
        const std::string& source;
        std::vector<Token> tokens;

        // private functions.
        void scanToken();
        void string();
        bool match(char expected);
        void addToken(TokenType t, std::string lexeme, unsigned int loc);
        void addToken(TokenType t);
        void short_comment();
        void long_comment();
        void number();
        void identifierOrKeyword();
        std::string strTolower(const std::string& s);
        bool isKeyword(const std::string& s);

        inline bool const isAtEnd() { return current > source.length(); }

        inline bool isDigit(char c) {
            return std::isdigit(static_cast<unsigned char>(c));
        }

        inline bool isAlpha(char c) {
            return (c >= 'a' && c <= 'z' ||
                    c >= 'A' && c <= 'Z' ||
                    c == '_');
        }
        
        inline bool isAlphanumeric(char c) {
            return isDigit(c) || isAlpha(c);
        }

        inline char peek() {
            if (isAtEnd()) return '\0';
            return source[current];
        }

        inline char previous() {
            return current>0 ? source[current-1] : source[0];
        }

        inline char advance() { return source[current++]; }

};
} //