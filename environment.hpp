#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>
#include <memory>


namespace cool {

template<class K, class V>
class Scope : public std::unordered_map<K, V> {

};


template<class K, class V>
class SymbolTable : public std::enable_shared_from_this<SymbolTable<K, V>> {
    private:
        Scope<K, V> scope;
        using PSymTable = std::shared_ptr<SymbolTable<K, V>>;
    public:
        SymbolTable() {enclosing = nullptr; scope{}; }

        SymbolTable(PSymbole enc) { enclosing = enc; scope{}; }

        void insert(K key, V value) {
            scope.insert(key, value);
        }

        void assign(K key, V value);
        V get(K key);
        void enterScope();
        void exitScope();
        void fatal_error(const std::string& msg) {
            std::cerr << msg  << "\n";
            exit(1);
        }
        PSymTable enclosing;

};

} // namespace cool