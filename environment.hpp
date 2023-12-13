#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>
#include <memory>
#include <list>

#include "list.hpp"


namespace cool {

template<class K, class V>
class Scope {
    private:
        std::unordered_map<K, std::unique_ptr<V>> scope;
        std::unique_ptr<Scope> enclosing;
    public:
        Scope(): scope({}), enclosing(nullptr) {}
        Scope(std::unique_ptr<Scope>&& encl): scope({}), enclosing{encl} {}
        void insert(K Key, std::unique_ptr<V> value) { scope.insert({key, std::move(value)})};
        
};

template<class K, class V>
class SymbolTable {
    private:
        using Scope_t = Scope<K, V>;
        Scope_t listScope;
    public:
        SymbolTable(): listScope() {}

        void insert(K key, std::unique_ptr<V> value) {
            if(listScope.empty()) {
                fatal_error("Insert: Can't add a symbol without a scope.");
            }
            listIter->insert({key, std::move(value)});
        }

        std::unique_ptr<V> get(K key) {
            for (auto iter = listScope.rbegin(); iter != listScope.rend(); ++iter) {
                auto value = iter->find(key);
                if (value != iter->end())
                    return std::move(value->second);
            }
            return nullptr; 
        }

        void enterScope() {
            listScope = ListScope(nullptr, std::move(listScope));
        }

        void exitScope() {
            if (listScope.empty()) {
                fatal_error("Exitscope: Can't remove scope from an empty symbol table.");
            }
            listScope = ListScope->tail();
        }

        void fatal_error(const std::string& msg) {
            std::cerr << msg  << "\n";
            exit(1);
        }

};

} // namespace cool