#include "environment.hpp"
#include <iostream>

#define PRINT(p) std::cout << ((p) ? std::to_string((*p)) : "null");
using namespace cool;

int main() {
    SymbolTable<int, int> symTab;
    auto a = std::make_unique<int>(3);
    auto b = std::make_unique<int>(4);
    auto c = std::make_unique<int>(5);
    symTab.enterScope();
    symTab.insert(1, std::move(a));
    symTab.enterScope();
    symTab.enterScope();
    symTab.enterScope();
    symTab.insert(2, std::move(b));
    symTab.insert(3, std::move(c));
    auto ret = symTab.get(1);
    PRINT(ret);
    ret = symTab.get(1);
    PRINT(ret) // should print 3
    ret = symTab.get(2);
    PRINT(ret) // should print 4
    ret = symTab.probe(3);
    PRINT(ret);
    symTab.exitScope();
    ret = symTab.get(2);
    PRINT(ret) // should print null
    symTab.exitScope();
    symTab.exitScope();
    symTab.exitScope();
    symTab.exitScope(); // error here
}
