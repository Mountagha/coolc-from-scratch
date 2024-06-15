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

using PStmt = std::unique_ptr<Stmt>;
using PExpr = std::unique_ptr<Expr>;

class Parser {
// recursive top down.
    public:
        Parser(std::vector<Token>& tokens);
        ~Parser();
        bool hasError();
        std::unique_ptr<Stmt> parse();
    
    private:
        // to represent id: token: expr into 1 object. (id: token) = formal.
        using letAssign = std::tuple<std::unique_ptr<Formal>, PExpr>; 
        using letAssigns = std::vector<letAssign>; // I know poor naming but hey.

        struct ParseError : std::runtime_error {
            // time to replace that.
            using std::runtime_error::runtime_error;
        };
        typeIdentifier typeId;
        std::vector<Token> tokens;
        unsigned int current;
        bool parseError;
        Program* program;

        PStmt parseProgram();
        PStmt parseClass(); 
        PExpr parseFeature();
        PExpr parseFormal();
        PExpr parseIf();
        PExpr parseWhile();
        PExpr parseLet();
        PExpr parseCase();
        PExpr parseBlock();
        PExpr parseExpression();
        PExpr parseAssignment();
        PExpr parseNotExpression();
        PExpr parseComparison();
        PExpr parseTerm();
        PExpr parseFactor();
        PExpr parseUnary();
        PExpr parseCall();
        std::vector<PExpr> parseArgs();
        PExpr parsePrimary();
        void synchronize();     // To get the parser unstuck.

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

        inline Token advance() {
            if (!isAtEnd()) current++;
            return previous();
        }

        inline bool isAtEnd() const {
            return peek().token_type == EOFILE;
        }

        inline Token peek() const {
            return tokens[current];
        }

        Token peek(unsigned int lookahead=0) {
            if (isAtEnd()) return tokens[current]; // We can't look ahead past the EOF.
            if (current+lookahead > tokens.size()-1) 
                return Token{EOFILE, "", tokens[tokens.size()-1].loc};
            return tokens[current+lookahead];
        }

        bool isCurToken(TokenType tt) const {
            return peek().token_type == tt;
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
            parseError = true;
            return ParseError{msg};
        }
};

} // namespace cool