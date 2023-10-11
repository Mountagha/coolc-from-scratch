#pragma once

#include "scanner.hpp"
#include "ast.hpp"
#include "scanner.hpp"
#include "error.h"
#include "type.hpp"


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

        PExpr parseExpression() {
            return parseAssignment();
        }

        PExpr parseAssignment() {
            PExpr expr = parseNotExpression();   
            if (match({ASSIGN})) {
                Token assign_ = previous();
                PExpr value = parseAssignment(); // Not sure if I'm handling left associativity correctly here.
                typeIdentifier typeId;
                if (typeId.identify(expr) == Type::Variable) {
                    Token name = static_cast<Variable*>(expr.get())->name;
                    return std::make_unique<Assign>(name, std::move(value));
                }
                error(assign_, "Invalid assignment Target");
            }
            return expr;
        }

        PExpr parseNotExpression() {
            PExpr expr = parseComparison();
            if (match ({NOT})) {
                expr = parseExpression();
            }
            return expr;
        }

        PExpr parseComparison() {
            PExpr expr = parseTerm();
            if (match ({LESS, LESS_EQUAL, EQUAL})) {
                Token operator_ = previous();
                PExpr rhs = parseTerm();
                expr = std::make_unique<Binary>(operator_, expr, rhs);
            }
            return expr;
        }

        PExpr parseTerm() {
            PExpr expr = parseFactor();
            if (match({PLUS, MINUS})) {
                Token operator_ = previous();
                PExpr rhs = parseFactor();
                expr = std::make_unique<Binary>(operator_, expr, rhs);
            }
            return expr;
        }

        PExpr parseFactor() {
            PExpr expr = parseUnary();
            if (match({STAR, SLASH})) {
                Token operator_ = previous();
                PExpr rhs = parseUnary();
                expr = std::make_unique<Binary>(operator_, expr, rhs);
            }
            return expr;
        }

        PExpr parseUnary() {
            if (match({TILDE, ISVOID})) {
                Token operator_ = previous();
                PExpr right = parseUnary(); 
                return std::make_unique<Unary>(operator_, std::move(right));
            }
            return parseCall();
        }

        PExpr parseCall() {
            PExpr expr = parsePrimary();
            while (true) {
                if (match({LEFT_PAREN})) {
                    expr = finishCall(expr);
                } else if (match({AT})) {
                    Token className = consume(IDENTIFIER, "Expect a valid class name after `@`");
                    consume(DOT, "Expect a dot after type identifier.");
                    Token id = consume(IDENTIFIER, "Expect an identifier after `.`.");
                    auto class_ = std::make_unique<Variable>(className);
                    expr = std::make_unique<Get>(id, std::move(expr), std::move(class_));
                } else if (match ({DOT})){
                    Token id = consume(IDENTIFIER, "Expect an identifier after `.`.");
                    expr = std::make_unique<Get>(id, std::move(expr));
                } else {
                    break;
                }
            }
        }

        PExpr finishCall(PExpr& callee) {
            std::vector<PExpr> arguments{};
            if (!check({RIGHT_PAREN})) {
                do {
                    arguments.push_back(parseExpression());
                }while(match({COMMA}));
            }
            Token paren = consume(RIGHT_PAREN, "Expect a right parenthesis at the end of a function call.");
            return std::make_unique<Call>(std::move(callee), paren, std::move(arguments));
        }

        PExpr parsePrimary() {

        }

        PExpr parseLet() {

        }

        PExpr parseCase() {

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