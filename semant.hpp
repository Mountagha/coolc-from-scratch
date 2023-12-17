#pragma once

#include <cstdlib>
#include <iostream>

#include "token.hpp"
#include "ast.hpp"
#include "utilities.hpp"
#include "environment.hpp"

namespace cool {

/*
    Symbols

    For convenience, a large number of symbols are predefined here.
    These symbols include the primitive type and method names, as well 
    as fixed names used by the runtime system.
*/

extern IdTable idtable;

static Token 
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copy,
    Int,
    in_int,
    in_string,
    IO,
    length,
    Main,
    main_meth,
    No_class, 
    No_type,
    Object,
    out_int,
    out_string,
    prim_slot,
    self,
    SELF_TYPE,
    Str,
    str_field,
    substr,
    type_name,
    val;

static void initialize_constants() {
    idtable.insert({arg, "arg"});
    idtable.insert({arg2, "arg2"});
    idtable.insert({Bool, "Bool"});
    idtable.insert({concat, "concat"});
    idtable.insert({cool_abort, "abort"});
    idtable.insert({copy, "copy"});
    idtable.insert({Int, "Int"});
    idtable.insert({in_int, "in_int"});
    idtable.insert({in_string, "in_string"});
    idtable.insert({IO, "IO"});
    idtable.insert({length, "length"});
    idtable.insert({Main, "Main"});
    idtable.insert({main_meth, "main"});

    //  _no_class is a symbol that can't be the name of
    // any user_defined class.

    idtable.insert({No_class, "_no_class"});
    idtable.insert({No_type, "_no_type"});
    idtable.insert({Object, "Object"});
    idtable.insert({out_int, "out_int"});
    idtable.insert({out_string, "out_string"});
    idtable.insert({prim_slot, "_prim_slot"});
    idtable.insert({self, "self"});
    idtable.insert({SELF_TYPE, "SELF_TYPE"});
    idtable.insert({Str, "String"});
    idtable.insert({str_field, "_str_field"});
    idtable.insert({substr, "substr"});
    idtable.insert({type_name, "type_name"});
    idtable.insert({val, "_val"});

}

class Semant : public StmtVisitor, public ExprVisitor {
    public:

        Semant() = default;

        void semant(std::unique_ptr<Expr>& expr) {
            expr->accept(this);
        }

        void semant(std::unique_ptr<Stmt>& expr) {
            expr->accept(this);
        }

        void visitPRogramStmt(Program* stmt) {

        }
    private:
        SymbolTable<Token, Class> classTable;

};

} // namespace cool