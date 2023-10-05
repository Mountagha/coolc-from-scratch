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
        using PExpr = std::unique_ptr<Expr>;
        using PStmt = std::unique_ptr<Stmt>;
        struct ParseError : std::runtime_error {
            using std::runtime_error::runtime_error;
        };
        std::vector<Token> tokens;
        unsigned int current;
        Program* program;

        PStmt parseProgram() {
            std::vector<std::unique_ptr<Class>> classes{};
            while (match({CLASS})) {
                classes.push_back(parseClass());
            }
            return std::make_unique<Program>(classes);
        }

        PStmt parseClass() {
            Token className = consume(IDENTIFIER, "Expect a class type after `class`.");
            PExpr superclas_;
            if(match({INHERITS})) { 
                Token superClassName = consume(IDENTIFIER, "Expect a Class Name after `inherits`");
                superclas_ = std::make_unique<Variable>(superClassName);
            }
            std::vector<std::unique_ptr<Feature>> features{};
            consume(LEFT_BRACE, "Expect a left brace.");
            while(match({IDENTIFIER})) {
                features.push_back(parseFeature());
            }
            consume(LEFT_BRACE, "Expect a right brace.");
            return std::make_unique<Class>(className, std::move(superclas_), std::move(features));
        }

        bool check(const TokenType& t) const {
            if (isAtEnd()) return false;
            return peek().token_type == t;
        }
        
        bool match(const std::vector<TokenType>& tts) {
            for(auto& tokenType: tts){
                if (check(tokenType)) {
                    advance();
                    return true;
                }
            }
            return false;
        }

        Token advance() {
            if (!isAtEnd()) current++;
            return previous();
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