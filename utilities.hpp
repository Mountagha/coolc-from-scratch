#pragma once

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include <memory>
#include <vector>
#include <stack>

#include "ast.hpp"
#include "token.hpp"
#include "constants.hpp"

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

// Inheritance graph
class InheritanceGraph {
    private:
        std::map<Token, Token> graph;
    public:
        InheritanceGraph() = default;
        void addEdge(const Token& a, const Token& b); // a inherits from b
        bool conform(Token a, Token b); // a is conform to b
        Token lca(Token a, Token b);    // common lowest ancestor of a and b
        bool isDGA();                  // whether the graph is acyclic or not.
        std::vector<Token> get_adjacents(Token& ) const ; // for every class get classes that inherits from it.
        std::vector<Token> DFS(Token&) const ; // Get a Depth Fist Search of the Inheritance Graph class. 
        bool is_leaf_class(Token&) const ;
        Token& lowest_child(Token& ) const; // return the lowest child (leaf) of a given node in the inheritance tree.
        std::map<Token, Token>& get_graph() { return graph; }
};

bool is_base_class(Class* class_);
bool is_base_function(Token& func_name);

} // namespace cool