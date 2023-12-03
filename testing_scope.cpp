#include "environment.hpp"
#include <iostream>

using namespace cool;

int main() {
    SymbolTable<int, int> symTab;
    auto a = std::make_unique<int>(3);
    auto b = std::make_unique<int>(4);
    auto c = std::make_unique<int>(5);
    symTab.enterScope();
    symTab.insert(1, a.get());
    auto ret = symTab.get(1);
    std::cout << *ret; 
    symTab.enterScope();
    symTab.insert(2, b.get());
    ret = symTab.get(1);
    std::cout << *ret; 
    symTab.exitScope();
    symTab.exitScope();
}