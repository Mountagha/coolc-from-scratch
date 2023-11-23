#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>
#include <list>

namespace cool {

template<class K, class V>
class Scope : public std::unordered_map<K, V> {

};

template<class K, class V>
class SymbolTable {
    private:
        using ScopeList = std::list
        std::vector<Scope<K, V>> symtab{};
        size_t curr_scope;
    public:
        SymbolTable() {symtab.push_back({}); curr_scope(0);}

        void enterScope() {symtab.push_back({}), curr_scope++; };

        void exitScope() {
            if (curr_scope == 0) 
                fatal_error("Cannot exit from the highest Scope.");
            curr_scope--;
        }

        void addEntry()
        void fatal_error(const std::string& msg) {
            std::cerr << msg  << "\n";
            exit(1);
        }

};

} // namespace cool