#pragma once

#include "scanner.hpp"
#include "ast.hpp"
#include "scanner.hpp"
#include "error.h"


#include <iostream> // debug purposes


namespace cool {


class Parser {
// recursive top down.
    public:
        Parser(std::vector<Token>& tokens);
        ~Parser();
    
    private:
        struct ParseError : std::runtime_error {
            using std::runtime_error::runtime_error;
        };
        std::vector<Token> tokens;
        unsigned int current;
        Program* program;

        Expr* parseProgram() {
            while (match({CLASS})) {


            }
        }

        bool check(const TokenType& t) const {
            if (isAtEnd()) return false;
            return peek().token_type == t;
        }
        
        bool match(const std::vector<TokenType>& tts)  const {
            for(auto& tokenType: tts){
                if (peek().token_type == tokenType) return true;
            }
            return false;
        }

        bool isAtEnd() const {
            return peek().token_type == EOFILE;
        }

        Token peek() const {
            return tokens[current];
        }

        Token previous() const {
            return tokens[current - 1];
        }
        inline Token consume(TokenType tt, const std::string& msg) {
            if (check(tt)) return advance();

            Token t = peek(); // cause of the passage by reference
            throw error(t, msg);
        }
        Token advance() {
            if(!isAtEnd()) current++;
            return previous();
        }

        ParseError error(const Token& t, const std::string& msg) {
            report(t, msg);

            return ParseError{msg};
        }



};

} // namespace cool