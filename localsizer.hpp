#pragma once

#include "ast.hpp"
#include <unordered_map>
#include <string>

namespace cool {

// This pass helps to collect the size of all local variables in every function.
// which in the case of cool are just let var. This will ease function codegen.

class LocalSizer: public StmtVisitor, public ExprVisitor {
    public:
        LocalSizer() = default;

        void computeSize(Expr* expr) {
            expr->accept(this);
        }

        void computeSize(Stmt* stmt) {
            stmt->accept(this);
        }

        size_t getFuncLocalSize(std::string& funcName) {
            if (sizes.find(funcName) != sizes.end())
                return sizes[funcName];
            // !TODO better error handling
            throw std::runtime_error("Unable to compute Local size for unknown function " + funcName);
        }

        
        void visitFeatureExpr(Feature* expr) {
            if (expr->featuretype == FeatureType::METHOD) {
                current_func = expr->id.lexeme;
                sizes.insert({current_func, 0});
                inside_func = true;
                expr->expr->accept(this);
                inside_func = false;
            }
        } 

        void visitLetExpr(Let* expr) {
            if (inside_func) {
                sizes[current_func] += expr->vecAssigns.size();               
                expr->body->accept(this);
            }
        }

        void visitFormalExpr(Formal* expr)  { }

        void visitAssignExpr(Assign* expr) { expr->expr->accept(this); }

        void visitIfExpr(If* expr) { 
            expr->cond->accept(this); 
            expr->thenBranch->accept(this);
            expr->elseBranch->accept(this);
        }

        void visitWhileExpr(While* expr) { 
            expr->cond->accept(this); 
            expr->expr->accept(this);
        }

        void visitBinaryExpr(Binary* expr) { 
            expr->lhs->accept(this); 
            expr->rhs->accept(this); 
        }

        void visitUnaryExpr(Unary* expr) { 
            expr->expr->accept(this); 
        }

        void visitVariableExpr(Variable* expr) { }

        void visitNewExpr(New* expr) { }  

        void visitBlockExpr(Block* expr) {
            for (auto& e: expr->exprs) {
                e->accept(this);
            }
        }

        void visitGroupingExpr(Grouping* expr) { expr->expr->accept(this); }

        void visitDispatchExpr(Dispatch* expr) { }

        void visitStaticDispatchExpr(StaticDispatch* expr) { }
        
        void visitLiteralExpr(Literal* expr) { }

        void visitCaseExpr(Case* expr) { 
            expr->expr->accept(this);
            for (auto& case_: expr->matches) {
                std::get<1>(case_).get()->accept(this);
            }
        }

        void visitProgramStmt(Program* stmt) { 
            for (auto& class_: stmt->classes) {
                class_->accept(this);
            }
        }

        void visitClassStmt(Class* stmt) { 
            for (auto& f: stmt->features) {
                f->accept(this);
            }
        }

    private:
        std::unordered_map<std::string, size_t> sizes{};
        std::string current_func;
        bool inside_func;
};

}