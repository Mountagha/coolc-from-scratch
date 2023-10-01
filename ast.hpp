#include <iostream>
#include <memory>
#include <vector>

#include "token.hpp"

namespace cool {


class ExprVisitor {

};

class StmtVisitor {

};

class Expr {
    public:
        Expr() = default;
        ~Expr() = default;
        virtual void accept(ExprVisitor* v) = 0;
};

class Stmt {
    public:
        Stmt() = default;
        ~Stmt() = default;
        virtual void accept(StmtVisitor* v) = 0;
};

class Class: public Stmt {
    public:
        Class(Token name_, std::unique_ptr<Class>&& superClass_, std::vector<std::unique_ptr<Feature>>&& features_){
            name = name_;
            superClass = std::move(superClass_);
            features = std::move(features_);
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
        Token id, type_;
};

class Assign: public Expr {
    public:
        Assign(Token id_, Token type__, std::unique_ptr<Expr>&& expr_) {
            id = id_;
            type_ = type__;
            expr = std::move(expr);
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
        std::unique_ptr<Expr> cond, thenBranch, elseBranch;
};

class While: public Expr {
    public:
        While(std::unique_ptr<Expr>&& cond_, std::unique_ptr<Expr>&& expr_) {
            cond = std::move(cond_);
            expr = std::move(expr_);
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
        std::unique_ptr<Expr> lhs, rhs;
        Token op;
};

class Unary: public Expr {
    public:
        Unary(Token op_, std::unique_ptr<Expr> expr_) {
            op = op_;
            expr = std::move(expr_);
        }
        Token op;
        std::unique_ptr<Expr> expr;
};


} //