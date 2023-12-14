#include "environment.hpp"
#include <iostream>

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
    auto ret = symTab.get(1);
    if (ret) 
    std::cout << *ret; // should print 3
    ret = symTab.get(2);
    if (ret)  
    std::cout << *ret; // should print 4
    symTab.exitScope();
    ret = symTab.get(2);
    if (ret)  
    std::cout << *ret; // should print nothing
    symTab.exitScope();
    symTab.exitScope();
    symTab.exitScope();
    symTab.exitScope(); // error here
}