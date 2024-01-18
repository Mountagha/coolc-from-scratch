#pragma once

#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include <memory>

#include "ast.hpp"
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


IdTable idtable;
StringTable stringtable;

void set_formals_type(std::vector<std::unique_ptr<Formal>>& formals) {
    for (auto& f: formals) {
        f->expr_type = f->type_;
    }
}

void set_features_type(std::vector<std::unique_ptr<Feature>>& features) {
    for (auto& f: features) {
        f->expr_type = f->type_;
    }
}


// later find a way to get rid of those globals.

static Class* curr_class;

/*
    Symbols

    For convenience, a large number of symbols are predefined here.
    These symbols include the primitive type and method names, as well 
    as fixed names used by the runtime system.
*/


static Token 
    arg{TokenType::IDENTIFIER, "arg"},
    arg2{TokenType::IDENTIFIER, "arg2"},
    Bool{TokenType::IDENTIFIER, "Bool"},
    concat{TokenType::IDENTIFIER, "concat"},
    cool_abort{TokenType::IDENTIFIER, "cool_abort"},
    copy{TokenType::IDENTIFIER, "copy"},
    Int{TokenType::IDENTIFIER, "Int"},
    in_int{TokenType::IDENTIFIER, "in_int"},
    in_string{TokenType::IDENTIFIER, "in_string"},
    IO{TokenType::IDENTIFIER, "IO"},
    length{TokenType::IDENTIFIER, "length"},
    Main{TokenType::IDENTIFIER, "Main"},
    main_meth{TokenType::IDENTIFIER, "main_meth"},
    No_class{TokenType::IDENTIFIER, "No_class"}, 
    No_type{TokenType::IDENTIFIER, "No_type"},
    Object{TokenType::IDENTIFIER, "Object"},
    out_int{TokenType::IDENTIFIER, "out_int"},
    out_string{TokenType::IDENTIFIER, "out_string"},
    prim_slot{TokenType::IDENTIFIER, "prim_slot"},
    self{TokenType::IDENTIFIER, "self"},
    SELF_TYPE{TokenType::IDENTIFIER, "SELF_TYPE"},
    Str{TokenType::IDENTIFIER, "Str"},
    str_field{TokenType::IDENTIFIER, "str_field"},
    substr{TokenType::IDENTIFIER, "substr"},
    type_name{TokenType::IDENTIFIER, "type_name"},
    val{TokenType::IDENTIFIER, "val"};

static void initialize_constants() {
    idtable.insert({"arg", arg});
    idtable.insert({"arg2", arg2});
    idtable.insert({"Bool", Bool});
    idtable.insert({"concat", concat});
    idtable.insert({"abort", cool_abort});
    idtable.insert({"copy", copy});
    idtable.insert({"Int", Int});
    idtable.insert({"in_int", in_int});
    idtable.insert({"in_string", in_string});
    idtable.insert({"IO", IO});
    idtable.insert({"length", length});
    idtable.insert({"Main", Main});
    idtable.insert({"main", main_meth});

    //  _no_class is a symbol that can't be the name of
    // any user_defined class.

    idtable.insert({"_no_class", No_class});
    idtable.insert({"_no_type", No_type});
    idtable.insert({"Object", Object});
    idtable.insert({"out_int", out_int});
    idtable.insert({"out_string", out_string});
    idtable.insert({"_prim_slot", prim_slot});
    idtable.insert({"self", self});
    idtable.insert({"SELF_TYPE", SELF_TYPE});
    idtable.insert({"String", Str});
    idtable.insert({"_str_field", str_field});
    idtable.insert({"substr", substr});
    idtable.insert({"type_name", type_name});
    idtable.insert({"_val", val});

}

// Inheritance graph
class InheritanceGraph {
    private:
        std::map<Token, Token> graph;
    public:
        InheritanceGraph() = default;
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
        current = curr_class->name;
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
        a = curr_class->name;
    if (b == SELF_TYPE)
        b = curr_class->name;
    
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