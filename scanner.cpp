#include "scanner.hpp"
#include "token.hpp"

namespace cool {

Scanner::Scanner(const std::string& source):
    source{source}, start{0}, current{0}, line{1} 
{}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    tokens.push_back(Token(EOFILE, "", line));
    return tokens;
}

void Scanner::scanToken() {
    // !TODO adding filename to string table now. later add
    // that info to the debugging info.
    stringtable().insert(curr_filename, Token{_NULL, curr_filename, 0});
    inttable().insert(std::to_string(curr_filename.size()), Token{NUMBER, std::to_string(curr_filename.size()), 0});
    char c = advance();
    switch(c) {
        // single char token
        case ')': addToken(RIGHT_PAREN); break;
        case '}': addToken(RIGHT_BRACE); break;
        case '{': addToken(LEFT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case ':': addToken(COLON); break;
        case '*': addToken(STAR); break;
        case '~': addToken(TILDE); break;
        case '/': addToken(SLASH); break;
        case '@': addToken(AT); break;

        // operators
        case '=': addToken(match('>') ? ARROW : EQUAL); break;
        case '<': addToken(match('-') ? ASSIGN : (match('=') ? LESS_EQUAL : LESS)); break;
        case '>': addToken(match('=') ? GREATER_EQUAL : GREATER); break;

        // left parenthese or long comment.
        case '(': {
            if (match('*'))
                long_comment();
            else
                addToken(LEFT_PAREN);
            break;
        }

        // Minus or short comment
        case '-': {
            if(match('-'))
                short_comment();
            else
                addToken(MINUS); break;
            break;
        }
        // whitespace
        case '\r':
        case '\t':
        case ' ' :
        case '\0':
            // skips whitespaces && nul characters
            break;
        case '\n':
            line++; break;
        case '"':
            string();
            break;
        default:
            if(isDigit(c))
                number();
            else if (isAlphanumeric(c))
                identifierOrKeyword();
            break;
    }
}

void Scanner::string() {
    while (!isAtEnd() && peek() != '"') {
        if (peek() == '\\') // handling escaped including \"
            advance();
        if(peek() == '\n') line++;
        advance();
    }
    if (isAtEnd()) {
        error(line, "Unterminated string...");
        return;
    }
    // consume the closing "
    advance();
    std::string lexeme = source.substr(start+1, current-start-2);
    addToken(STRING, lexeme, line);
    stringtable().insert(lexeme, Token{STRING, lexeme, line});
    inttable().insert(std::to_string(lexeme.size()), Token{NUMBER, std::to_string(lexeme.size()), line});
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] == expected) {
        current++;
        return true;
    }
    return false;
}
void Scanner::addToken(TokenType t, std::string lexeme, unsigned int loc) {
    tokens.push_back(Token(t, lexeme, loc));
}

void Scanner::addToken(TokenType t) {
    std::string lexeme = source.substr(start, current-start);
    addToken(t, lexeme, line);
}

void Scanner::short_comment() {
    while (!isAtEnd() && peek() != '\n'){
        if (peek() == '\n') line++;
        advance();
    }
}

void Scanner::long_comment() {
    bool isBalanced = false;
    std::stack<int> nesting{};
    nesting.push(1);
    while (!isBalanced && !isAtEnd()){
        if (match('(') && match('*'))
            nesting.push(1);
        else if(match('*') && match(')'))
            nesting.pop();
        if (nesting.empty())
            isBalanced = true;
        if (peek() == '\n') line++;
        advance();
    }
    if (!isBalanced)
        error(line, "Unterminated long comments.");
}

void Scanner::number() {
    while(isDigit(peek())) {
        advance();
    }
    std::string lexeme = source.substr(start, current-start);
    addToken(NUMBER, lexeme, line);
    inttable().insert(lexeme, Token{NUMBER, lexeme, line});
}

void Scanner::identifierOrKeyword() {
    while(isAlphanumeric(peek())) {
        advance();
    }
    std::string lexeme = source.substr(start, current-start);
    if (isKeyword(lexeme))
        addToken(keywordsMap.at(strTolower(lexeme)), lexeme, line);
    else
        addToken(IDENTIFIER, lexeme, line);
        idtable().insert(lexeme, Token{IDENTIFIER, lexeme, line});
        inttable().insert(std::to_string(lexeme.size()), Token{NUMBER, std::to_string(lexeme.size()), line});
}

std::string Scanner::strTolower(const std::string& s) {
    std::string lowerStr = s;
    std::transform(lowerStr.cbegin(), lowerStr.cend(), lowerStr.begin(),
        [](unsigned char c) {return std::tolower(c); });
    return lowerStr;
}

bool Scanner::isKeyword(const std::string& s) {
    if (keywordsMap.find(strTolower(s)) != keywordsMap.end())
        return true;
    return false;
}


}
