#pragma once

#include <cstdlib>
#include <iostream>
#include <ostream>

#include "token.hpp"
#include "utilities.hpp"
#include "environment.hpp"

namespace cool {

/*
    Symbols

    For convenience, a large number of symbols are predefined here.
    These symbols include the primitive type and method names, as well 
    as fixed names used by the runtime system.
*/


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

            construct_ctables(stmt);

            check_inheritance(stmt);

            // check program
            
        }

        void visitClassStmt(Class* stmt) {
            
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
            error_stream << "Error at line : " << c.loc << " " << msg << "\n";
            return semant_error();
        }


    private:
        SymbolTable<std::string, Class> classTable;
        unsigned int semant_errors;
        std::ostream& error_stream;
        bool class_main_exist{false};

        bool check_parents(Program* stmt) {
            // make sure all classes have parent class.
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

        bool check_DAG(Program* stmt) {
            
            bool ret = true;
            // Build the inheritance_graph

            InheritanceGraph g;
            g.addEdge(IO, Object);
            g.addEdge(Int, Object);
            g.addEdge(Str, Object);
            g.addEdge(Bool, Object);

            for (auto& class_: stmt->classes) {
                g.addEdge(class_->name, class_->superClass->name);
            }

            if(!g.isDGA()) {
                semant_error(curr_class->name, "Cyclic inheritance detected!"); // find better way to find the error position.
                ret = false;
            }
            return ret;
        }
        void construct_ctables(Program* stmt) {

            classTable.enterScope();

            for (auto& class_: stmt->classes) {
                Token class_name, parent_name;
                class_name = class_->name;
                curr_class = class_.get();
                std::cout << class_name;
                parent_name = class_->superClass->name;
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

                classTable.insert(class_name.lexeme, class_.get());
            }

            //classTable.exitScope();

        }
        void check_inheritance(Program* stmt) {
            // check if parents are defined.
            if (!check_parents(stmt)) return;

            // check if the graph does not contains cycle. 
            check_DAG(stmt);
        }

        
};

} // namespace cool