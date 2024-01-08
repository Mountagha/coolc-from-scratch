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

// the base classes.
std::unique_ptr<Class> Object_class, IO_class, Int_class, Bool_class, Str_class;


IdTable idtable;
StringTable stringtable;

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

static void install_basic_classes() {
    // The tree package uses these globals to annotate the classes built below.
    // curr_lineno  = 0;
    stringtable.insert({"<basic_class", {TokenType::IDENTIFIER, "<basic_class>"}});
    
    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.
    
    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    // 
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.

    std::vector<std::unique_ptr<Formal>> abort_formals;
    std::vector<std::unique_ptr<Formal>> typename_formals;
    std::vector<std::unique_ptr<Formal>> copy_formals;
    std::vector<std::unique_ptr<Feature>> feats = {
        std::make_unique<Feature>(cool_abort, abort_formals, Object, nullptr, FeatureType::METHOD),
        std::make_unique<Feature>(type_name, typename_formals, Str, nullptr, FeatureType::METHOD),
        std::make_unique<Feature>(copy, copy_formals, SELF_TYPE, nullptr, FeatureType::METHOD)
    };

    auto Object_class_ = std::make_unique<Class> (
        Object,
        std::make_unique<Variable>(No_class),
        std::move(feats)
    );
    Object_class = std::move(Object_class_);

    // 
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    std::vector<std::unique_ptr<Formal>> out_string_formals = { std::make_unique<Formal>(arg, Str) };
    std::vector<std::unique_ptr<Formal>> out_int_formals = { std::make_unique<Formal>(arg, Int) };
    std::vector<std::unique_ptr<Formal>> in_string_formals;
    std::vector<std::unique_ptr<Formal>> in_int_formals;

    std::vector<std::unique_ptr<Feature>> io_feats = {
        std::make_unique<Feature>(out_string, out_string_formals, SELF_TYPE, nullptr, FeatureType::METHOD),
        std::make_unique<Feature>(out_int, out_int_formals, SELF_TYPE, nullptr, FeatureType::METHOD),
        std::make_unique<Feature>(in_string, in_string_formals, Str, nullptr, FeatureType::METHOD),
        std::make_unique<Feature>(in_int, in_int_formals, Int, nullptr, FeatureType::METHOD),
    };
    auto IO_class_ = std::make_unique<Class>(
        IO,
        std::make_unique<Variable>(Object),
        std::move(io_feats)
    ); 
    IO_class = std::move(IO_class_);

     //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer. 
    //
    std::vector<std::unique_ptr<Formal>> str_attr_formals;
    std::vector<std::unique_ptr<Feature>> int_feats = { 
        std::make_unique<Feature>(val, str_attr_formals, prim_slot, nullptr, FeatureType::ATTRIBUT), 
    };
    auto Str_class_ = std::make_unique<Class>(
        Int,
        std::make_unique<Variable>(Object),
        std::move(int_feats)
    );
    Str_class = std::move(Str_class_);

    //
    // Bool also has only the "val" slot.
    //
    std::vector<std::unique_ptr<Formal>> bool_attr_formals;
    std::vector<std::unique_ptr<Feature>> bool_feats = { 
        std::make_unique<Feature>(val, str_attr_formals, prim_slot, nullptr, FeatureType::ATTRIBUT), 
    };
    auto Bool_class_ = std::make_unique<Class>(
        Bool,
        std::make_unique<Variable>(Object),
        std::move(int_feats)
    );
    Bool_class = std::move(Bool_class_);

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //       

    std::vector<std::unique_ptr<Formal>> val_formals = { };
    std::vector<std::unique_ptr<Formal>> str_field_formals = { };
    std::vector<std::unique_ptr<Formal>> length_formals = { };
    std::vector<std::unique_ptr<Formal>> concat_formals = { std::make_unique<Formal>(arg, Int), };
    std::vector<std::unique_ptr<Formal>> substr_formals = { 
        std::make_unique<Formal>(arg, Int),
        std::make_unique<Formal>(arg2, Int), 
    };
    std::vector<std::unique_ptr<Feature>> str_features = {
        std::make_unique<Feature>(val, val_formals, Int, nullptr, FeatureType::ATTRIBUT),
        std::make_unique<Feature>(str_field, str_field_formals, prim_slot, nullptr, FeatureType::METHOD),
        std::make_unique<Feature>(length, length_formals, Int, nullptr, FeatureType::METHOD),
        std::make_unique<Feature>(concat, concat_formals, Str, nullptr, FeatureType::METHOD),
        std::make_unique<Feature>(substr, substr_formals, Str, nullptr, FeatureType::METHOD),
    };
    auto Str_class_ = std::make_unique<Class>(
        Str,
        std::make_unique<Variable>(Object),
        std::move(str_features)
    );
    Str_class = std::move(Str_class);

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