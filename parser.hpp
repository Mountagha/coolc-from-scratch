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
        Token current;
        Program* program;

        Expr* parseProgram() {
            while (match(CLASS)) {

            }
        }

};

} // namespace cool