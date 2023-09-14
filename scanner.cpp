#include "scanner.hpp"
#include "token.hpp"

namespace cool {
Scanner::Scanner(const std::string& source):
    source{source}, start{0}, current{0}, line{0} 
{}

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    tokens.push_back(Token(EOFILE, "", line));
    return tokens;
}

}
