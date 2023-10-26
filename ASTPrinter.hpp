/* A pretty printer of the ast generated by the parser. */
#pragma once

#include "ast.hpp"
#include <iostream>
namespace cool {

class ASTPrinter : public ExprVisitor, public StmtVisitor  {
    public:
        ASTPrinter() = default;

        void print(std::unique_ptr<Expr>& expr) {
            expr->accept(this);
            std::cout << ast_string << '\n';
        }

        void print(std::unique_ptr<Stmt>& stmt) {
            stmt->accept(this);
            std::cout << ast_string << '\n';
        }

        void visitFeatureExpr(Feature* expr);
        void visitFormalExpr(Formal* expr);
        void visitAssignExpr(Assign* expr);
        void visitIfExpr(If* expr);
        void visitWhileExpr(While* expr);
        void visitBinaryExpr(Binary* expr);
        void visitUnaryExpr(Unary* expr);
        void visitVariableExpr(Variable* expr); 
        void visitCallExpr(Call* expr);
        void visitBlockExpr(Block* expr);
        void visitGroupingExpr(Grouping* expr);
        void visitGetExpr(Get* expr);
        void visitLiteralExpr(Literal* expr);
        void visitLetExpr(Let* expr);
        void visitCaseExpr(Case* expr);


        void visitProgramStmt(Program* stmt);
        void visitClassStmt(Class* smtt);

    public:
        std::string ast_string;
        
};

} // namespace cool