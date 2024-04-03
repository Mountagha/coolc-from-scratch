#pragma once

#include "token.hpp"
#include "tokentable.hpp"

namespace cool {

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
    cool_abort{TokenType::IDENTIFIER, "abort"},
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
    prim_slot{TokenType::IDENTIFIER, "_prim_slot"},
    self{TokenType::IDENTIFIER, "self"},
    SELF_TYPE{TokenType::IDENTIFIER, "SELF_TYPE"},
    Str{TokenType::IDENTIFIER, "String"},
    str_field{TokenType::IDENTIFIER, "_str_field"},
    substr{TokenType::IDENTIFIER, "substr"},
    type_name{TokenType::IDENTIFIER, "type_name"},
    val{TokenType::IDENTIFIER, "_val"};

static void initialize_constants() {
    idtable().insert("arg", arg);
    idtable().insert("arg2", arg2);
    idtable().insert("Bool", Bool);
    idtable().insert("concat", concat);
    idtable().insert("abort", cool_abort);
    idtable().insert("copy", copy);
    idtable().insert("Int", Int);
    idtable().insert("in_int", in_int);
    idtable().insert("in_string", in_string);
    idtable().insert("IO", IO);
    idtable().insert("length", length);
    idtable().insert("Main", Main);
    idtable().insert("main", main_meth);

    //  _no_class is a symbol that can't be the name of
    // any user_defined class.

    idtable().insert("_no_class", No_class);
    idtable().insert("_no_type", No_type);
    idtable().insert("Object", Object);
    idtable().insert("out_int", out_int);
    idtable().insert("out_string", out_string);
    idtable().insert("_prim_slot", prim_slot);
    idtable().insert("self", self);
    idtable().insert("SELF_TYPE", SELF_TYPE);
    idtable().insert("String", Str);
    idtable().insert("_str_field", str_field);
    idtable().insert("substr", substr);
    idtable().insert("type_name", type_name);
    idtable().insert("_val", val);

}


}