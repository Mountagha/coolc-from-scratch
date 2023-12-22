#pragma once

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include "token.hpp"

namespace cool {

/*
    There are three kinds of string table entries:
        a true string, a string representation of an identifier
        and a string representation of an integer.

    Having separate tables is convenient for code generation. Different
    data definitions are generated for string constants (StringEntry) and
    integer constants (IntEntry). Identifiers (IdEntry) don't produce
    static data definitions.

    code_def and code_ref are used by the code to produce definitions
    and references (respectively) to constants.
*/

using IdTable = std::unordered_map<std::string, Token>;
using StringTable = std::unordered_map<std::string, Token>;
using IntTable = std::unordered_map<std::string, Token>;

extern Token curr_class;

// Inheritance graph
class InheritanceGraph {
    private:
        std::map<Token, Token> graph;
    public:
        void addEdge(const Token& a, const Token& b); // a inherits from b
        bool conform(const Token& a, const Token& b); // a is conform to b
        Token lca(Token a, Token b);    // common lowest ancestor of a and b
        bool isDGA();                  // whether the graph is acyclic or not.
};

void InheritanceGraph::addEdge(const Token& a, const Token& b) {
    if (graph.count(a) != 0) return;
    graph[a] = b;
}

bool InheritanceGraph::isDGA() {
    // object is the root of the inheritance graph.
    Token Object{TokenType::IDENTIFIER, "Object"};
    for(auto it = graph.begin(); it != graph.end(); it++) {
        Token current = it->first;
        Token next = it->second;
        std::set<Token> visited;
        visited.insert(current);
        while (current != Object) {
            if(visited.count(next) != 0)
                return false;
            visited.insert(next);
            current = next;
            next = graph[next];
        }
    }
    return true;
}

bool InheritanceGraph::conform(const Token& a, const Token& b) {
    Token current = a;
    if (a == b) 
        return true;
    if (current == SELF_TYPE)
        current = curr_class.name;
    while (current != Object) {
        if(current == b)
            return true;
        current = graph[current]; 
    }
    return current == b;
}

Token InheritanceGraph::lca(Token a, Token b) {
    if (a == b)
        return a;
    if (a == SELF_TYPE)
        a = curr_class.name;
    if (b == SELF_TYPE)
        b = curr_class.name;
    
    int high_a = 0, high_b = 0;
    Token temp_a = a, temp_b = b;

    while(temp_a != Object) {
        temp_a = graph[temp_a];
        high_a++;
    }

    while (temp_b != Object) {
        temp_b = graph[temp_b];
        high_b++;
    }

    temp_a = a, temp_b = b;

    if (high_a > high_b) {
        for (int i = high_a - high_b; i>0; i--) {
            temp_a = graph[temp_a];
        }
    } else {
        for (int i = high_b - high_a; i>0; i--) {
            temp_b = graph[temp_b];
        }
    }

    while (temp_a != temp_b) {
        temp_a = graph[temp_a];
        temp_b = graph[temp_b];
    }
    return temp_a;
}


} // namespace cool