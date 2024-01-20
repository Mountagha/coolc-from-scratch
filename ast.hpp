#pragma once

#include <memory>
#include <vector>

#include "token.hpp"
#include "object.hpp"

namespace cool {

class Expr;
class Stmt;
class Program;
class Class;
class Feature;
class Formal;
class Assign;
class If;
class While;
class Let;
class Binary;
class Unary;
class Block;
class Variable;
class New;
class Grouping;
class Literal;
class Case;
class StaticDispatch;
class Dispatch;

using PExpr = std::unique_ptr<Expr>;
using letAssign = std::tuple<Token, Token, PExpr>; // to represent id: token: expr into 1 object.
using letAssigns = std::vector<letAssign>; // I know poor naming but hey.

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
        virtual void visitNewExpr(New* expr) = 0;
        virtual void visitBlockExpr(Block* expr) = 0;
        virtual void visitGroupingExpr(Grouping* expr) = 0;
        virtual void visitStaticDispatchExpr(StaticDispatch* expr) = 0;
        virtual void visitDispatchExpr(Dispatch* expr) = 0;
        virtual void visitLiteralExpr(Literal* expr) = 0;
        virtual void visitLetExpr(Let* expr) = 0;
        virtual void visitCaseExpr(Case* expr) = 0;

};

class StmtVisitor {
    public:
        virtual void visitProgramStmt(Program* stmt) = 0;
        virtual void visitClassStmt(Class* stmt) = 0;
};

class Expr {
    public:
        Expr() = default;
        ~Expr() = default;
        virtual void accept(ExprVisitor* visitor) = 0;
        Token expr_type;
};

class Stmt {
    public:
        Stmt() = default;
        ~Stmt() = default;
        virtual void accept(StmtVisitor* visitor) = 0;
};

class Program: public Stmt {
    public:
        Program(std::vector<std::unique_ptr<Class>>&& classes_) {
            classes = std::move(classes_);
        }
        void accept(StmtVisitor* visitor) {
            visitor->visitProgramStmt(this);
        }
        std::vector<std::unique_ptr<Class>> classes;
};

class Class: public Stmt {
    public:
        Class(Token name_, Token superClass_, std::vector<std::unique_ptr<Feature>>&& features_){
            name = name_;
            superClass = superClass_;
            features = std::move(features_);
        }
        void accept(StmtVisitor* visitor) {
            visitor->visitClassStmt(this);
        }
        Token name, superClass;
        std::vector<std::unique_ptr<Feature>> features; 
};

class Feature: public Expr {
    public:
        Feature(Token id_, std::vector<std::unique_ptr<Formal>>&& formals_, Token type__, std::unique_ptr<Expr>&& expr_, FeatureType ft){
            id = id_;
            formals = std::move(formals_);
            type_ = type__;
            expr = std::move(expr_);
            featuretype = ft;
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitFeatureExpr(this);
        }
        Token id;
        std::vector<std::unique_ptr<Formal>> formals;
        Token type_;
        std::unique_ptr<Expr> expr;
        FeatureType featuretype;
         
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
        Assign(Token id_, std::unique_ptr<Expr>&& expr_) {
            id = id_;
            expr = std::move(expr_);
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitAssignExpr(this);
        }
        Token id;
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

class New: public Expr {
    // Can't stuff this neither into Variable nor Unary so make its own class.
    public:
        New(Token type__) {
            type_ = type__;
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitNewExpr(this);
        }
        Token type_;
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

class StaticDispatch: public Expr {
    public:
        StaticDispatch(Token callee_name_, std::unique_ptr<Expr>&& expr_, 
        std::unique_ptr<Variable>&& class__ ,std::vector<std::unique_ptr<Expr>>&& args_) {
            callee_name = callee_name_;
            expr = std::move(expr_);
            class_ = std::move(class__);
            args = std::move(args_);
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitStaticDispatchExpr(this);
        }
        std::unique_ptr<Expr> expr;
        std::unique_ptr<Variable> class_;
        std::vector<std::unique_ptr<Expr>> args;
        Token callee_name;
};

class Dispatch: public Expr {
    public:
        Dispatch(Token callee_name_, std::unique_ptr<Expr>&& expr_, 
        std::vector<std::unique_ptr<Expr>>&& args_) {
            callee_name = callee_name_;
            expr = std::move(expr_);
            args = std::move(args_);
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitDispatchExpr(this);
        }
        std::unique_ptr<Expr> expr;
        std::vector<std::unique_ptr<Expr>> args;
        Token callee_name;
};


class Literal: public Expr {
    public:
        Literal(CoolObject&& o) {
            object = o;
        }
        void accept(ExprVisitor* visitor) {
            visitor->visitLiteralExpr(this);
        }
        CoolObject object;
};

class Let: public Expr {
    public: 
        Let(letAssigns&& vecAssigns_, std::unique_ptr<Expr> body_) {
            vecAssigns = std::move(vecAssigns_);
            body = std::move(body_);
        }
        
        void accept(ExprVisitor* visitor) {
            visitor->visitLetExpr(this);
        }
        letAssigns vecAssigns;
        std::unique_ptr<Expr> body;
};

class Case: public Expr {
    public: 
        Case(letAssigns&& matches_, std::unique_ptr<Expr> expr_) {
            matches = std::move(matches_);
            expr = std::move(expr_);
        }
        
        void accept(ExprVisitor* visitor) {
            visitor->visitCaseExpr(this);
        }
        letAssigns matches;
        std::unique_ptr<Expr> expr;
};

} //