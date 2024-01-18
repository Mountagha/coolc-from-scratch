#pragma once

#include "ast.hpp"

namespace cool {

enum class Type {
    Variable, // to be completed
    Dispatch,
    StaticDispatch,
};

class typeIdentifier: public StmtVisitor, public ExprVisitor {
    public:
        typeIdentifier() = default;

        Type identify(std::unique_ptr<Expr>& expr) {
            expr->accept(this);
            return type;
        }
        Type identify(std::unique_ptr<Stmt>& stmt) {
            stmt->accept(this);
            return type; 
        }

        void visitFeatureExpr(Feature* expr) {} 
        void visitFormalExpr(Formal* expr)  {}
        void visitAssignExpr(Assign* expr) {}
        void visitIfExpr(If* expr) {}
        void visitWhileExpr(While* expr) {}
        void visitBinaryExpr(Binary* expr) {}
        void visitUnaryExpr(Unary* expr) {}
        void visitVariableExpr(Variable* expr) { type = Type::Variable; }
        void visitNewExpr(New* expr) { }  
        void visitBlockExpr(Block* expr) {}
        void visitGroupingExpr(Grouping* expr) {}
        void visitDispatchExpr(Dispatch* expr) { type = Type::Dispatch; }
        void visitStaticDispatchExpr(StaticDispatch* expr) { type = Type::StaticDispatch; }
        void visitLiteralExpr(Literal* expr) {}
        void visitLetExpr(Let* expr) {}
        void visitCaseExpr(Case* expr) {}


        void visitProgramStmt(Program* stmt) {}
        void visitClassStmt(Class* smtt) {}

    private:
        Type type;
};

}