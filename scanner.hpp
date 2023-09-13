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
            char c = advance()
            switch(c) {
                case '(': addToken()
            } 
        }

        char advance() const { return source[current++]; }

        void addToken(TokenType tokenType, std::string lexeme, unsigned int loc) {
            tokens.push_back(Token(tokenType, lexeme, loc));
        }

};
} //