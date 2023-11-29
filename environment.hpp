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
class SymbolTable {
    private:
        using Scope = std::unordered_map<K, V>; 
        using ListScope = std::list<Scope>;
        using ListIter = typename ListScope::iterator;
        ListScope listScope;
        ListIter listIter;
    public:
        SymbolTable() {}

        void insert(K key, V value) {
            listIter->insert({key, value});
        }

        V get(K key) {
            for (auto iter = listScope.rbegin(); iter != listScope.rend(); ++iter) {
                auto value = iter->find(key);
                if (value != iter->end())
                    return value->second;
            }
            return V(); 
        }

        void enterScope() {
            listScope.push_back({});
            listIter = listScope.end();
            --listIter;
        }

        void exitScope() {
            if (listScope.empty()) {
                fatal_error("Exitscope: Can't remove scope from an empty symbol table.");
            }
            --listIter;
        }

        void fatal_error(const std::string& msg) {
            std::cerr << msg  << "\n";
            exit(1);
        }

};

} // namespace cool