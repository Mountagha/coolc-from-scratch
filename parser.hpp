#pragma once

#include "scanner.hpp"
#include "ast.hpp"
#include "scanner.hpp"
#include "error.h"
#include "type.hpp"
#include "object.hpp"


#include <iostream> // debug purposes
#include <tuple>
#include <memory>


namespace cool {


class Parser {
// recursive top down.
    public:
        Parser(std::vector<Token>& tokens);
        ~Parser();
        std::unique_ptr<Stmt> parse();
    
    private:
        using PExpr = std::unique_ptr<Expr>;
        using PStmt = std::unique_ptr<Stmt>;
        using letAssign = std::tuple<Token, Token, PExpr>; // to represent id: token: expr into 1 object.
        using letAssigns = std::vector<letAssign>; // I know poor naming but hey.

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
                consume(SEMICOLON, "Expect a `;` at the end of class declaration.");
            }
            return std::make_unique<Program>(std::move(classes));
        }

        PStmt parseClass() {
            Token className = consume(IDENTIFIER, "Expect a class type after `class`.");
            std::unique_ptr<Variable> superclass;
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
            return std::make_unique<Class>(className, std::move(superclass), std::move(features));
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
            return std::make_unique<Feature>(id, std::move(formals), type_, std::move(expr));
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

        PExpr parseLet() {
            letAssigns vecAssigns{}; 
            do {
                Token id = consume(IDENTIFIER, "Expect a valid identifier.");
                Token type_ = consume(IDENTIFIER, "Expect a valid type.");
                PExpr expr;
                if (match({ASSIGN})) expr = parseExpression();
                vecAssigns.push_back(std::make_tuple(id, type_, std::move(expr)));
            }while(match({COMMA}) && !isAtEnd());
            consume(IN, "Expect in after let assigns.");
            PExpr body = parseExpression();
            return std::make_unique<Let>(std::move(vecAssigns), std::move(body));
        }

        PExpr parseCase() {
            PExpr caseExpr = parseExpression();
            letAssigns matches{};
            consume(OF, "Expect an of keyword after case expression.");
            do {
                Token id = consume(IDENTIFIER, "Expect a valid identifier");
                Token type_ = consume(IDENTIFIER, "Expect a valid type.");
                consume(ARROW, "Expect an arrow in case expression.");
                PExpr expr = parseExpression();
                matches.push_back(std::make_tuple(id, type_, std::move(expr)));
            } while(match({SEMICOLON}) && !isAtEnd());
            consume(ESAC, "Expect an `esac` keyword at the end of a case expression.");
            return std::make_unique<Case>(std::move(matches), std::move(caseExpr));
        }

        PExpr parseBlock() {
            std::vector<PExpr> exprs{};
            do {
                exprs.push_back(parseExpression());
            } while(match({COMMA}) && !isAtEnd());
            consume(RIGHT_BRACE, "Expect a `{` after a block.");
            return std::make_unique<Block>(std::move(exprs));
        }

        PExpr parseExpression() {
            return parseAssignment();

            if (match({LEFT_BRACE})) return parseBlock(); 
            if (match({IF})) return parseIf();
            if (match({WHILE})) return parseWhile();
            if (match({CASE})) return parseCase();
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
                expr = std::make_unique<Binary>(operator_, std::move(expr), std::move(rhs));
            }
            return expr;
        }

        PExpr parseTerm() {
            PExpr expr = parseFactor();
            if (match({PLUS, MINUS})) {
                Token operator_ = previous();
                PExpr rhs = parseFactor();
                expr = std::make_unique<Binary>(operator_, std::move(expr), std::move(rhs));
            }
            return expr;
        }

        PExpr parseFactor() {
            PExpr expr = parseUnary();
            if (match({STAR, SLASH})) {
                Token operator_ = previous();
                PExpr rhs = parseUnary();
                expr = std::make_unique<Binary>(operator_, std::move(expr), std::move(rhs));
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
            return expr;
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
            if (match ({NEW})) {
                Token type_ = consume(IDENTIFIER, "Expect a valide class type after new");
                return std::make_unique<Variable>(type_);
            }
            if (match ({ISVOID})) return parseExpression();
            if (match ({IDENTIFIER})) return std::make_unique<Variable>(previous());
            if (match ({NUMBER})) return std::make_unique<Literal>(CoolObject(std::stoi(previous().lexeme)));
            if (match ({STRING})) return std::make_unique<Literal>(CoolObject(previous().lexeme));
            if (match ({TRUE})) return std::make_unique<Literal>(CoolObject(true));
            if (match ({FALSE})) return std::make_unique<Literal>(CoolObject(false));

            throw error(peek(), "Expect an expression.");
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

        Token peek(int lookahead) {
            if (isAtEnd()) return tokens[current]; // We can't look ahead past the EOF.
            return tokens[current+lookahead];
        }

        Token previous() const {
            return tokens[current - 1];
        }
        inline Token consume(TokenType tt, const std::string& msg) {
            if (check(tt)) return advance();

            Token t = peek(); // cause of the passage by reference
            throw error(t, msg);
        }

        ParseError error(const Token& t, const std::string& msg) {
            report(t, msg);

            return ParseError{msg};
        }

        // To get the parser unstuck.
        void synchronize() {
            advance(); 
            while(!isAtEnd()) {
                switch (peek().token_type){
                    case IDENTIFIER:
                        if(peek(1).token_type == COLON || peek(1).token_type == LEFT_PAREN)
                            return; // going to the next feature.
                    case LET:
                    case CLASS:
                    case CASE:
                    case SEMICOLON:
                        return;

                }
                advance();
            }
        }



};

} // namespace cool