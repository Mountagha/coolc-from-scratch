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
                auto class_ = parseClass();
                // Probably a better way to do next line.
                classes.push_back(std::unique_ptr<Class>(static_cast<Class*>(class_.release())));
            }
            return std::make_unique<Program>(classes);
        }

        PStmt parseClass() {
            Token className = consume(IDENTIFIER, "Expect a class type after `class`.");
            PExpr superclass;
            if(match({INHERITS})) { 
                Token superClassName = consume(IDENTIFIER, "Expect a Class Name after `inherits`");
                superclass = std::make_unique<Variable>(superClassName);
            }
            std::vector<std::unique_ptr<Feature>> features{};
            consume(LEFT_BRACE, "Expect a left brace.");
            while(match({IDENTIFIER})) {
                auto feature = parseFeature();
                features.push_back(std::unique_ptr<Feature>(static_cast<Feature*>(feature.release())));
            }
            consume(LEFT_BRACE, "Expect a right brace.");
            return std::make_unique<Class>(className, std::move(superclass), features);
        }

        PExpr parseFeature() {
            Token id = consume(IDENTIFIER, "Expecting an identifier.");
            std::vector<std::unique_ptr<Formal>> formals{};
            PExpr expr;
            if (match({LEFT_PAREN})) {
                do {
                    auto formal = parseFormal(); 
                    formals.push_back(std::unique_ptr<Formal>(static_cast<Formal*>(formal.release())));
                }while(match({COMMA}));
            }
            consume(COLON, "Expecting a colon.");
            Token type_ = consume(IDENTIFIER, "Expecting a type.");
            if (match({LEFT_BRACE})) {
                expr = parseExpression();
                consume(RIGHT_BRACE, "Expecting a right brace.");
            } else if (match({ASSIGN})) {
                expr = parseExpression();
            }
            return std::make_unique<Feature>(id, formals, type_, std::move(expr));
        }

        PExpr parseFormal() {
            Token id = consume(IDENTIFIER, "Expecting an identifier.");
            consume(COLON, "Expecting a Colon.");
            Token type_ = consume(IDENTIFIER, "Expecting a type.");
            return std::make_unique<Formal>(id, type_);
        }

        PExpr parseIf() {
            consume(IF, "Expecting `if` keyword.");
            PExpr cond = parseExpression();
            consume(THEN, "Expecting `then` keyword.");
            PExpr thenBranch = parseExpression();
            consume(ELSE, "Expecting `else` keyword.");
            PExpr elseBranch = parseExpression();
            consume(FI, "Expecing `fi` keyword.");
            return std::make_unique<If>(std::move(cond), std::move(thenBranch), std::move(elseBranch));
        }

        PExpr parseWhile() {
            consume(WHILE, "Expecting `while` keyword.");
            PExpr cond = parseExpression();
            consume(LOOP, "Expecting `loop` keyword.");
            PExpr expr = parseExpression();
            consume(POOL, "Expecting `pool` keyword.");
            return std::make_unique<While>(std::move(cond), std::move(expr));
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