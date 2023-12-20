#pragma once

#include <cstdlib>
#include <iostream>
#include <ostream>

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

IdTable idtable;

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

class Semant : public StmtVisitor, public ExprVisitor {
    public:

        Semant(std::ostream& out=std::cerr): error_stream{out} {} 

        void semant(std::unique_ptr<Expr>& expr) {
            expr->accept(this);
        }

        void semant(std::unique_ptr<Stmt>& expr) {
            expr->accept(this);
        }

        void visitProgramStmt(Program* stmt) {

            initialize_constants();

            classTable.enterScope();

            for (auto& class_ : stmt->classes) {
                class_->accept(this);
            }

            check_parents(stmt);
            
        }

        void visitClassStmt(Class* stmt) {

            Token class_name, parent_name;
            class_name = stmt->name;
            std::cout << class_name;
            parent_name = stmt->superClass->name;
            if (class_name == Main) {
                class_main_exist = true;
            }
            if (class_name == SELF_TYPE) {
                semant_error(class_name, "Cannot define a class named SELF_TYPE.");
            }

            if (classTable.get(class_name.lexeme)) {
                semant_error(class_name, class_name.lexeme + " already defined.");
            }

            // We can't inherit from the basic class in Cool
            if (parent_name == Main || parent_name == Int ||
                parent_name == Str  || parent_name == SELF_TYPE) {
                    semant_error(class_name, "Class " + class_name.lexeme + "cannot inherits from " + parent_name.lexeme + "\n");

            }

            classTable.insert(class_name.lexeme, stmt);

        }

        virtual void visitFeatureExpr(Feature* expr) {}
        virtual void visitFormalExpr(Formal* expr) {}
        virtual void visitAssignExpr(Assign* expr) {}
        virtual void visitIfExpr(If* expr) {}
        virtual void visitWhileExpr(While* expr) {}
        virtual void visitBinaryExpr(Binary* expr) {}
        virtual void visitUnaryExpr(Unary* expr) {}
        virtual void visitVariableExpr(Variable* expr) {}
        virtual void visitCallExpr(Call* expr) {}
        virtual void visitBlockExpr(Block* expr) {}
        virtual void visitGroupingExpr(Grouping* expr) {}
        virtual void visitGetExpr(Get* expression) {}
        virtual void visitLiteralExpr(Literal* expr) {}
        virtual void visitLetExpr(Let* expr) {}
        virtual void visitCaseExpr(Case* expr) {}


        std::ostream& semant_error() {
            semant_errors++;
            return error_stream;
        }

        std::ostream& semant_error(Token& c, const std::string& msg) {
            error_stream << "Error at : " << c.loc << msg << "\n";
            return semant_error();
        }


    private:
        SymbolTable<std::string, Class> classTable;
        unsigned int semant_errors;
        std::ostream& error_stream;
        bool class_main_exist{false};

        bool check_parents(Program* stmt) {
            // make sure all class have parent class.
            bool ret{true};
            Token className, parentClassName;
            for (auto& class_ : stmt->classes) {
                className = class_->name;
                parentClassName = class_->superClass->name;
                if (!classTable.get(parentClassName.lexeme) && (parentClassName == No_class)) {
                    semant_error(class_->name, "Parent class " + parentClassName.lexeme + " of class " + className.lexeme + " is not defined.");
                    ret = false;
                }
            }
            return ret;
        }
         void check_inheritance(Program* stmt) {

         }

};

} // namespace cool