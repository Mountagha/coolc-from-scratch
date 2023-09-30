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
        Class(Token name_, std::unique_ptr<Class> superClass_, std::vector<std::unique_ptr<Feature>>&& features_){
            name = name_;
            superClass = std::move(superClass_);
            features = std::move(features_);
        }
        Token name;
        std::unique_ptr<Class> superClass;
        std::vector<std::unique_ptr<Feature>> features; 
};

class Feature: public Expr {

};

} //