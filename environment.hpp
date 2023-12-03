#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>
#include <memory>
#include <list>

//#include "list.hpp"


namespace cool {

template<class K, class V>
class Scope {
    private:
        std::unordered_map<K, V*> scope;
    public:
        std::shared_ptr<Scope> enclosing;
        Scope(): scope({}), enclosing(nullptr) {}
        // Scope(const Scope& other) {
        //     for (auto& elt: scope) {
        //         other.scope.insert({key, std::make_unique<V>(*elt)});
        //     }
        // }
        Scope(std::shared_ptr<Scope>& encl): scope({}), enclosing{encl} {}

        void insert(K key, V* value) { scope.insert({key, value}); }

        V* get(K key) {
            auto value = scope.find(key);
            if (value != scope.end())
                return value->second;
            return nullptr; 
        }

};

template<class K, class V>
class SymbolTable {
    private:
        using Scope_t = std::shared_ptr<Scope<K, V>>;
        Scope_t listScope;
    public:
        SymbolTable(): listScope(nullptr) {}

        void insert(K key, V* value) {
            if(listScope == nullptr) {
                fatal_error("Insert: Can't add a symbol without a scope.");
            }
            listScope->insert(key, value);
        }

        V* get(K key) {
            auto v = listScope->get(key);
            if (v != nullptr)
                return v;
            return listScope->enclosing->get(key);
        }

        void enterScope() {
            listScope = std::make_shared<Scope<K, V>>(listScope);
        }

        void exitScope() {
            if (listScope == nullptr) {
                fatal_error("Exitscope: Can't remove scope from an empty symbol table.");
            }
            listScope = listScope->enclosing;
        }

        void fatal_error(const std::string& msg) {
            std::cerr << msg  << "\n";
            exit(1);
        }

};

} // namespace cool