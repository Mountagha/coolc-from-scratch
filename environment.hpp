#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>
#include <memory>
#include <list>


namespace cool {

template<class K, class V>
class Scope : public std::unordered_map<K, V> {

};


template<class K, class V>
class SymbolTable : public std::enable_shared_from_this<SymbolTable<K, V>> {
    private:
        using ListScope = std::list<Scope<K, V>>;
        std::shared_ptr<ListScope> listScope;
        std::unique_ptr<Scope<K, V>> cur_scope;
    public:
        SymbolTable() { cur_scope{}; enclosing {nullptr}; }

        //SymbolTable(PSymbole enc) { enclosing = enc; scope{}; }

        void insert(K key, V value) {
            cur_scope.insert(key, value);
        }

        V get(K key) {
            for (auto iter = listScope.rbegin(); iter != listScope.rend(); ++it)
            auto value = scope.find(key);
            if (value != scope.end()) {
                return value;
            }

            if (enclosing) return enclosing->get(key);
        }

        void fatal_error(const std::string& msg) {
            std::cerr << msg  << "\n";
            exit(1);
        }

};

template<class K, class V>
class ScopedEnvironment {
    private:
        Using PSymTable = std::shared_ptr<SymbolTable<K, V>>;
        PSymtable backup_env;
        PSymtable

}

} // namespace cool