#pragma once

#include <unordered_map>
#include <vector>
#include <iostream>
#include <memory>
#include <list>


namespace cool {

template<class K, class V>
class Scope : public std::enable_shared_from_this<Scope<K, V>> {
    private:
        std::unordered_map<K, V> scope{};
        std::shared_ptr<Scope> enclosing;
    public:
        Scope(): enclosing(nullptr) {}
        Scope(std::shared_ptr<Scope>& encl): enclosing(encl) {}

        std::shared_ptr<Scope> getEnclosing() { return this->shared_from_this()->enclosing; }

        void insert(K key, V value) { scope.insert({key, value}); }

        V* get(K key) {
            auto value = scope.find(key);
            if (value != scope.end())
                return &value->second;
            return nullptr; 
        }

};

template<class K, class V>
class Scope<K, V*> : public std::enable_shared_from_this<Scope<K, V*>> {
    private:
        std::unordered_map<K, V*> scope{};
        std::shared_ptr<Scope> enclosing;
    public:
        Scope<K, V*>(): enclosing(nullptr) {}
        Scope<K, V*>(std::shared_ptr<Scope>& encl): enclosing(encl) {}

        std::shared_ptr<Scope<K, V*>> getEnclosing() { return this->shared_from_this()->enclosing; }

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

        void insert(K key, V value) {
            if(listScope == nullptr) {
                fatal_error("Insert: Can't add a symbol without a scope.");
            }
            listScope->insert(key, value);
        }

        V* get(K key) {
            if (listScope == nullptr)
                fatal_error("No scope available");
            auto v = listScope->get(key);
            if (v != nullptr)
                return v;
            Scope_t current = listScope->getEnclosing();
            while (current != nullptr ) { 
                v = current->get(key); 
                if(v != nullptr) return v;
                current = current->getEnclosing();
            }
            return nullptr;
        }

        V* probe(K key) {
            auto v = listScope->get(key);
            return v ? v : nullptr;
        }

        void enterScope() {
            listScope = std::make_shared<Scope<K, V>>(listScope);
        }

        void exitScope() {
            if (listScope == nullptr) {
                fatal_error("Exitscope: Can't remove scope from an empty symbol table.");
            }
            listScope = listScope->getEnclosing();
        }

        void fatal_error(const std::string& msg) {
            std::cerr << msg  << "\n";
            exit(1);
        }

};


template<class K, class V>
class SymbolTable<K, V*> {
    private:
        using Scope_t = std::shared_ptr<Scope<K, V*>>;
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
            if (listScope == nullptr)
                fatal_error("No scope available");
            auto v = listScope->get(key);
            if (v != nullptr)
                return v;
            Scope_t current = listScope->getEnclosing();
            while (current != nullptr ) { 
                v = current->get(key); 
                if(v != nullptr) return v;
                current = current->getEnclosing();
            }
            return nullptr;
        }

        V* probe(K key) {
            auto v = listScope->get(key);
            return v ? v : nullptr;
        }

        void enterScope() {
            listScope = std::make_shared<Scope<K, V*>>(listScope);
        }

        void exitScope() {
            if (listScope == nullptr) {
                fatal_error("Exitscope: Can't remove scope from an empty symbol table.");
            }
            listScope = listScope->getEnclosing();
        }

        void fatal_error(const std::string& msg) {
            std::cerr << msg  << "\n";
            exit(1);
        }

};


} // namespace cool