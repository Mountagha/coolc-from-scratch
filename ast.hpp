#include <iostream>
#include <memory>
#include <vector>

#include "token.hpp"

namespace cool {


class ExprVisitor {
    public:
        virtual void visitFeatureExpr(Feature* expr) = 0;
        virtual void visitFormalExpr(Formal* expr) = 0;
        virtual void visitAssignExpr(Assign* expr) = 0;
        virtual void visitIfExpr(If* expr) = 0;
        virtual void visitWhileExpr(While* expr) = 0;
        virtual void visitBinaryExpr(Binary* expr) = 0;
        virtual void visitUnaryExpr(Unary* expr) = 0;
        virtual void visitVariableExpr(Variable* expr) = 0;
        virtual void visitCallExpr(Call* expr) = 0;
        virtual void visitBlockExpr(Block* expr) = 0;
        virtual void visitGroupingExpr(Grouping* expr) = 0;

};

class StmtVisitor {
    public:
        virtual void visitProgramStmt(Program* stmt) = 0;
        virtual void visitClassStmt(Class* smtt) = 0;
};

class Expr {
    public:
        Expr() = default;
        ~Expr() = default;
        virtual void accept(ExprVisitor* visitor) = 0;
};

class Stmt {
    public:
        Stmt() = default;
        ~Stmt() = default;
        virtual void accept(StmtVisitor* visitor) = 0;
};

class Program: public Stmt {
    public:
        Program(std::unique_ptr<Class>&& classes_) {
            classes = std::move(classes);
        }
        void accept(StmtVisitor* visitor) {
            visitor->visitProgramStmt(this);
        }
        std::vector<Class> classes;
};

class Class: public Stmt {
    public:
        Class(Token name_, std::unique_ptr<Class>&& superClass_, std::vector<std::unique_ptr<Feature>>&& features_){
            name = name_;
            superClass = std::move(superClass_);
            features = std::move(features_);
        }
        void accept(StmtVisitor* visitor) {
            visitor->visitClassStmt(this);
        }
        Token name;
        std::unique_ptr<Class> superClass;
        std::vector<std::unique_ptr<Feature>> features; 
};

class Feature: public Expr {
    public:
        Feature(Token id_, std::vector<Formal>&& formals_, Token type__, std::unique_ptr<Expr>&& expr_){
            id = id_;
            formals = std::move(formals_);
            type_ = type__;
            expr = std::move(expr_);
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitFeatureExpr(this);
        }
        Token id;
        std::vector<Formal> formals;
        Token type_;
        std::unique_ptr<Expr> expr;
         
};

class Formal : public Expr {
    public:
        Formal(Token id_, Token type__) {
            id = id_;
            type_ = type__;
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitFormalExpr(this);
        }
        Token id, type_;
};

class Assign: public Expr {
    public:
        Assign(Token id_, Token type__, std::unique_ptr<Expr>&& expr_) {
            id = id_;
            type_ = type__;
            expr = std::move(expr);
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitAssignExpr(this);
        }
        Token id;
        Token type_;
        std::unique_ptr<Expr> expr;
};

class If: public Expr {
    public:
        If(std::unique_ptr<Expr>&& cond_, std::unique_ptr<Expr>&& then_, std::unique_ptr<Expr>&& else_) {
            cond = std::move(cond_);
            thenBranch = std::move(then_);
            elseBranch = std::move(else_);
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitIfExpr(this);
        }
        std::unique_ptr<Expr> cond, thenBranch, elseBranch;
};

class While: public Expr {
    public:
        While(std::unique_ptr<Expr>&& cond_, std::unique_ptr<Expr>&& expr_) {
            cond = std::move(cond_);
            expr = std::move(expr_);
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitWhileExpr(this);
        }
        std::unique_ptr<Expr> cond, expr;
};

class Binary: public Expr {
    public:
        Binary(Token op_, std::unique_ptr<Expr>&& lhs_, std::unique_ptr<Expr>&& rhs_) {
            op = op_;
            lhs = std::move(lhs_);
            rhs = std::move(rhs_);
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitBinaryExpr(this);
        }
        std::unique_ptr<Expr> lhs, rhs;
        Token op;
};

class Unary: public Expr {
    public:
        Unary(Token op_, std::unique_ptr<Expr>&& expr_) {
            op = op_;
            expr = std::move(expr_);
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitUnaryExpr(this);
        }
        Token op;
        std::unique_ptr<Expr> expr;
};

class Variable: public Expr {
    public:
        Variable(Token name_) {
            name = name_;
        } 
        void accept(ExprVisitor* visitor) {
            visitor->visitVariableExpr(this);
        }
        Token name;
};

class Call: public Expr {
    public:
        Call(std::vector<std::unique_ptr<Expr>>&& args_) {
           args = std::move(args_); 
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitCallExpr(this);
        }
        std::vector<std::unique_ptr<Expr>> args;
};

class Block: public Expr {
    public:
        Block(std::vector<std::unique_ptr<Expr>>&& exprs_) {
            exprs = std::move(exprs_);
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitBlockExpr(this);
        }
        std::vector<std::unique_ptr<Expr>> exprs;
};

class Grouping: public Expr {
    public: 
        Grouping(std::unique_ptr<Expr>&& expr_) {
           expr = std::move(expr_);
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitGroupingExpr(this);
        }
        std::unique_ptr<Expr> expr;
};



} //