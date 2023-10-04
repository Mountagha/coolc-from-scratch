#include "scanner.hpp"
#include "ast.hpp"
#include "scanner.hpp"


#include <iostream> // debug purposes


namespace cool {


class Parser {
// recursive top down.
    public:
        Parser(std::vector<Token>& tokens);
        ~Parser();
    
    private:
        std::vector<Token> tokens;
        unsigned int current;
        Program* program;

        Expr* parseProgram() {
            while (match(CLASS)) {

            }
        }

    bool match(const TokenType& t) const {
        if (isAtEnd()) return false;
        return peek().token_type == t;
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

};

} // namespace cool