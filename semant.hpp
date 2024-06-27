#pragma once

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <tuple>

#include "token.hpp"
#include "utilities.hpp"
#include "constants.hpp"
#include "environment.hpp"

namespace cool {


class Semant : public StmtVisitor, public ExprVisitor {
    public:

        Semant(std::ostream& out=std::cerr): error_stream{out}, semant_errors{0} {} 

        void semant(std::unique_ptr<Expr>& expr) {
            expr->accept(this);
        }

        void semant(std::unique_ptr<Stmt>& expr) {
            expr->accept(this);
        }

        inline bool hasError() const {
            return semant_errors > 0;
        }

        void visitProgramStmt(Program* stmt);
        void visitClassStmt(Class* stmt);
        virtual void visitFeatureExpr(Feature* expr);
        virtual void visitFormalExpr(Formal* expr);
        virtual void visitAssignExpr(Assign* expr);
        virtual void visitIfExpr(If* expr);
        virtual void visitWhileExpr(While* expr);
        virtual void visitBinaryExpr(Binary* expr);
        virtual void visitUnaryExpr(Unary* expr);
        virtual void visitVariableExpr(Variable* expr);
        virtual void visitNewExpr(New* expr);
        virtual void visitBlockExpr(Block* expr);
        virtual void visitGroupingExpr(Grouping* expr);
        virtual void visitStaticDispatchExpr(StaticDispatch* expr);
        virtual void visitDispatchExpr(Dispatch* expr);
        virtual void visitLiteralExpr(Literal* expr);
        virtual void visitLetExpr(Let* expr);
        virtual void visitCaseExpr(Case* expr);
        void check_attribut(Feature* expr);
        void check_method(Feature* expr);

        Feature* get_feature(Class* stmt, const std::string& name, FeatureType ft);

        // !TODO: better error handling. later!
        std::ostream& semant_error();

        void fatal_semant_error(Token &c, const std::string& msg);

        std::ostream& semant_error(Token& c, const std::string& msg);
        // some getters
        SymbolTable<std::string, Class* >* get_classtable() { return &classTable; }
        InheritanceGraph* get_inheritancegraph() { return &g; }


    private:
        SymbolTable<std::string, Class*> classTable;
        SymbolTable<std::string, Token> symboltable;
        SymbolTable<std::string, Token> earger_features; // allow use before declarations.
        // the base classes.
        std::unique_ptr<Class> Object_class, IO_class, Int_class, Bool_class, Str_class;
        unsigned int semant_errors;
        Class* curr_class;
        std::ostream& error_stream;
        bool class_main_exist{false};
        InheritanceGraph g;

        bool check_parents(Program* stmt);
        bool check_DAG(Program* stmt);
        void construct_ctables(Program* stmt);
        void gather_features(Program *stmt);
        void multiple_definition_of_method_checks(Program *stmt);
        void check_inheritance(Program* stmt);
        void install_basic_classes();
        void set_formals_type(std::vector<std::unique_ptr<Formal>>& formals);
        void set_features_type(std::vector<std::unique_ptr<Feature>>& features);

        bool conform(Token a, Token b);
        Token LCA(Token a, Token b);



       
};

} // namespace cool