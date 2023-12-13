#include "environment.hpp"
#include <iostream>

using namespace cool;

int main() {
    SymbolTable<int, int> sT;
    sT.enterScope();
    sT.insert(2, std::make_unique<int>(5));
    sT.enterScope();
    sT.insert(1, std::make_unique<int>(1));
    auto x = sT.get(2);
    std::cout << *x;
    sT.exitScope();
    x = sT.get(1);
    if (x) std::cout << *x;
    x = sT.get(2);
    if (x) std::cout << *x;
    sT.enterScope();
    sT.insert(6, std::make_unique<int>(10));
    x = sT.get(2);
    if (x) std::cout << *x;
    return 0;
}