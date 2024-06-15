#include "parser.hpp"

namespace cool {

Parser::Parser(std::vector<Token>& tokens_): tokens{tokens_}, current{0}, parseError{false} {}
Parser::~Parser() = default;
bool Parser::hasError() { return parseError; }

PStmt Parser::parse() {
    try {
        return parseProgram();
    } catch (ParseError error) {
        synchronize();
        return PStmt{}; // to please to C++ compiler.
    }
}

PStmt Parser::parseProgram() {
    std::vector<std::unique_ptr<Class>> classes{};
    while(!isAtEnd()){
        auto class_ = parseClass();
        // Probably a better way to do next line.
        classes.push_back(std::unique_ptr<Class>(static_cast<Class*>(class_.release())));
        consume(SEMICOLON, "Expect `;` at the end of a class definition.");
    }
    return std::make_unique<Program>(std::move(classes));
}

PStmt Parser::parseClass() {
    consume(CLASS, "Expect the keyword `class` at the beginning of class definition.");
    Token className = consume(IDENTIFIER, "Expect a class type after `class`.");
    Token superClassName;
    if(match({INHERITS})) 
        superClassName = consume(IDENTIFIER, "Expect a Class Name after `inherits`");
    else
        superClassName = Token(TokenType::IDENTIFIER, "Object");
    std::vector<std::unique_ptr<Feature>> features{};
    consume(LEFT_BRACE, "Expect a left brace at the beginning of a class definition.");
    while (isCurToken(IDENTIFIER)){
        PExpr feature = parseFeature();
        features.push_back(std::unique_ptr<Feature>(static_cast<Feature*>(feature.release())));
        consume(SEMICOLON, "Expect a `;` at the end of a feature definition.");
    }
    consume(RIGHT_BRACE, "Expect a right brace after class definition.");
    return std::make_unique<Class>(className, superClassName, std::move(features));
}

PExpr Parser::parseFeature() {
    Token id = consume(IDENTIFIER, "Expecting an identifier.");
    std::vector<std::unique_ptr<Formal>> formals{};
    PExpr expr;
    FeatureType featuretype = check(LEFT_PAREN) ? FeatureType::METHOD : FeatureType::ATTRIBUT;
    if (match({LEFT_PAREN}) && !match({RIGHT_PAREN})) {
        do {
            auto formal = parseFormal(); 
            formals.push_back(std::unique_ptr<Formal>(static_cast<Formal*>(formal.release())));
        }while(match({COMMA}) && !isAtEnd());
        consume(RIGHT_PAREN, "Expecting a `)` after params listing.");
    }
    consume(COLON, "Expecting a colon.");
    Token type_ = consume(IDENTIFIER, "Expecting a type.");
    if (match({LEFT_BRACE})) {
        expr = parseExpression();
        consume(RIGHT_BRACE, "Expecting a right brace.");
    } else if (match({ASSIGN})) {
        expr = parseExpression();
    }
    return std::make_unique<Feature>(id, std::move(formals), type_, std::move(expr), featuretype);
}

PExpr Parser::parseFormal() {
    Token id = consume(IDENTIFIER, "Expecting an identifier.");
    consume(COLON, "Expecting a Colon.");
    Token type_ = consume(IDENTIFIER, "Expecting a type.");
    return std::make_unique<Formal>(id, type_);
}

PExpr Parser::parseIf() {
    PExpr cond = parseExpression();
    consume(THEN, "Expecting `then` keyword.");
    PExpr thenBranch = parseExpression();
    consume(ELSE, "Expecting `else` keyword.");
    PExpr elseBranch = parseExpression();
    consume(FI, "Expecing `fi` keyword.");
    return std::make_unique<If>(std::move(cond), std::move(thenBranch), std::move(elseBranch));
}

PExpr Parser::parseWhile() {
    PExpr cond = parseExpression();
    consume(LOOP, "Expecting `loop` keyword.");
    PExpr expr = parseExpression();
    consume(POOL, "Expecting `pool` keyword.");
    return std::make_unique<While>(std::move(cond), std::move(expr));
}

PExpr Parser::parseLet() {
    letAssigns vecAssigns{}; 
    do {
        Token id = consume(IDENTIFIER, "Expect a valid identifier.");
        consume(COLON, "Expect `:` after identifier in `Let expression`.");
        Token type_ = consume(IDENTIFIER, "Expect a valid type.");
        PExpr expr;
        if (match({ASSIGN})) expr = parseExpression();
        vecAssigns.push_back(std::make_tuple(
            std::make_unique<Formal>(id, type_),
            std::move(expr))
        );
    }while(match({COMMA}) && !isAtEnd());
    consume(IN, "Expect `in` keyword after let assigns.");
    PExpr body = parseExpression();
    return std::make_unique<Let>(std::move(vecAssigns), std::move(body));
}

PExpr Parser::parseCase() {
    PExpr caseExpr = parseExpression();
    letAssigns matches{};
    consume(OF, "Expect an of keyword after case expression.");
    while(isCurToken(IDENTIFIER)) {
        Token id = consume(IDENTIFIER, "Expect a valid identifier");
        consume(COLON, "Expect `:` after identifier in `Case expression`.");
        Token type_ = consume(IDENTIFIER, "Expect a valid type.");
        consume(ARROW, "Expect an arrow in case expression.");
        matches.push_back(std::make_tuple(
            std::make_unique<Formal>(id, type_), 
            parseExpression())
        );
        consume(SEMICOLON, "Expect a `;` after expression within Case.");
    }
    consume(ESAC, "Expect an `esac` keyword at the end of a case expression.");
    return std::make_unique<Case>(std::move(matches), std::move(caseExpr));
}

PExpr Parser::parseBlock() {
    std::vector<PExpr> exprs{};
    while (!match({RIGHT_BRACE}) && !isAtEnd()){
        exprs.push_back(parseExpression());
        consume(SEMICOLON, "Expect a `;` after an expression.");
    }
    return std::make_unique<Block>(std::move(exprs));
}

PExpr Parser::parseExpression() {

    return parseAssignment();
}

PExpr Parser::parseAssignment() {
    PExpr expr = parseNotExpression();   
    if (match({ASSIGN})) {
        Token assign_ = previous();
        PExpr value = parseAssignment(); // Not sure if I'm handling left associativity correctly here.
        if (typeId.identify(expr) == Type::Variable) {
            Token name = static_cast<Variable*>(expr.get())->name;
            return std::make_unique<Assign>(name, std::move(value));
        }
        error(assign_, "Invalid assignment Target");
    }
    return expr;
}

PExpr Parser::parseNotExpression() {
    // this should be stuffed into the parseUnary func
    // but due to precedence I had to keep it appart.
    if (match ({NOT})) {
        Token operator_not = previous();
        PExpr expr = parseExpression();
        return std::make_unique<Unary>(operator_not, std::move(expr));
    }
    return parseComparison();
}

PExpr Parser::parseComparison() {
    PExpr expr = parseTerm();
    while (match ({LESS, LESS_EQUAL, EQUAL})) {
        Token operator_ = previous();
        PExpr rhs = parseTerm();
        expr = std::make_unique<Binary>(operator_, std::move(expr), std::move(rhs));
    }
    return expr;
}

PExpr Parser::parseTerm() {
    PExpr expr = parseFactor();
    while (match({PLUS, MINUS})) {
        Token operator_ = previous();
        PExpr rhs = parseFactor();
        expr = std::make_unique<Binary>(operator_, std::move(expr), std::move(rhs));
    }
    return expr;
}

PExpr Parser::parseFactor() {
    PExpr expr = parseUnary();
    while (match({STAR, SLASH})) {
        Token operator_ = previous();
        PExpr rhs = parseUnary();
        expr = std::make_unique<Binary>(operator_, std::move(expr), std::move(rhs));
    }
    return expr;
}

PExpr Parser::parseUnary() {
    if (match({TILDE, ISVOID})) {
        Token operator_ = previous();
        PExpr right = parseUnary(); 
        return std::make_unique<Unary>(operator_, std::move(right));
    }
    return parseCall();
}

PExpr Parser::parseCall() {
    PExpr expr = parsePrimary();
    while (true) {
        if (check(LEFT_PAREN)){
            if (typeId.identify(expr) == Type::Dispatch || typeId.identify(expr) == Type::StaticDispatch) {
                Token id;
                // redundant checking by heyyy.
                if (typeId.identify(expr) == Type::Dispatch)
                    id = static_cast<Dispatch*>(expr.release())->callee_name;
                else 
                    id = static_cast<StaticDispatch*>(expr.release())->callee_name;
                expr = std::make_unique<Dispatch>(id, std::move(expr), parseArgs());
            } else {
                Token self_tok = Token{TokenType::IDENTIFIER, "self"}; // !TODO: find a way to add line number later.
                PExpr self_expr = std::make_unique<Variable>(self_tok);
                // if we enter this branch then parsePrimary returned a Variable Expr which is the nanme of the func.
                Token id = static_cast<Variable*>(expr.release())->name;
                expr = std::make_unique<Dispatch>(id, std::move(self_expr), parseArgs());
            }
        } else if (match({AT})) { // static dispatch
            Token className = consume(IDENTIFIER, "Expect a valid class name after `@`");
            consume(DOT, "Expect a dot after type identifier.");
            Token id = consume(IDENTIFIER, "Expect an identifier after `.`.");
            expr = std::make_unique<StaticDispatch>(id, std::move(expr), className, parseArgs());
        } else if (match ({DOT})) { // dynamic dispatch
            Token id = consume(IDENTIFIER, "Expect an identifier after `.`.");
            expr = std::make_unique<Dispatch>(id, std::move(expr), parseArgs());
        } else {
            break;
        } 
    }
    return expr;
}

std::vector<PExpr> Parser::parseArgs() {
    consume(LEFT_PAREN, "Expect '(' at call beginning.");
    std::vector<PExpr> arguments{};
    if (!check({RIGHT_PAREN})) {
        do {
            arguments.push_back(parseExpression());
        }while(match({COMMA}) && !isAtEnd());
    }
    Token paren = consume(RIGHT_PAREN, "Expect a right parenthesis at the end of a function call.");
    return arguments;
}

PExpr Parser::parsePrimary() {
    if (match ({NEW})) {
        Token type_ = consume(IDENTIFIER, "Expect a valide class type after new");
        return std::make_unique<New>(type_);
    }
    if (match ({ISVOID})) return parseExpression();
    if (match ({IDENTIFIER})) return std::make_unique<Variable>(previous());
    if (match ({NUMBER})) return std::make_unique<Literal>(CoolObject(std::stoi(previous().lexeme)));
    if (match ({STRING})) return std::make_unique<Literal>(CoolObject(previous().lexeme));
    if (match ({TRUE})) return std::make_unique<Literal>(CoolObject(true));
    if (match ({FALSE})) return std::make_unique<Literal>(CoolObject(false));

    if (match({LEFT_BRACE})) return parseBlock(); 
    if (match({IF})) return parseIf();
    if (match({WHILE})) return parseWhile();
    if (match({CASE})) return parseCase();
    if (match({LET})) return parseLet();

    // Grouping (expr)
    if (match({LEFT_PAREN})) {
        PExpr expr = parseExpression();
        consume(RIGHT_PAREN, "Expect ')' at the end of a grouping expression.");
        return std::make_unique<Grouping>(std::move(expr));
    }

    throw error(peek(), "Expect an expression.");
}

// To get the parser unstuck.
void Parser::synchronize() {
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


}; // end of cool namespace