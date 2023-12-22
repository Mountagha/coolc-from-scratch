#pragma once

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <map>
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




} // namespace cool