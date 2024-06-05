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
            if (local_func_sizes.find(funcName) != local_func_sizes.end())
                return local_func_sizes[funcName];
            // !TODO better error handling
            throw std::runtime_error("Unable to compute Local size for unknown function " + funcName);
        }

        size_t getClassLocalSize(std::string& className) {
            if (local_class_sizes.find(className) != local_class_sizes.end())
                return local_class_sizes[className];
            // !TODO better error handling
            throw std::runtime_error("Unable to compute Local size for unknown class " + className);
        }


        
        void visitFeatureExpr(Feature* expr) {
            if (expr->featuretype == FeatureType::METHOD) {
                current_func = expr->id.lexeme;
                local_func_sizes.insert({current_func, 0});
                inside_func = true;
                expr->expr->accept(this);
                inside_func = false;
            } else {    // attributes affect local class size
                if (expr->expr)
                    expr->expr->accept(this);
            }
        } 

        void visitLetExpr(Let* expr) {
            if (inside_func) {
                local_func_sizes[current_func] += expr->vecAssigns.size();               
            } else { // count towards class size.
                local_class_sizes[current_class] += expr->vecAssigns.size();
            }
            expr->body->accept(this);
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
            if (inside_case)
                curr_case_size += 1;
            else 
                curr_case_size = 1;
            expr->expr->accept(this);
            for (auto& case_: expr->matches) {
                std::get<1>(case_).get()->accept(this);
            }
            if (inside_func) {
                local_func_sizes[current_func] = local_func_sizes[current_func] > curr_case_size ?
                        local_func_sizes[current_func] : curr_case_size;
            } else {
                local_class_sizes[current_class] = local_class_sizes[current_class] > curr_case_size ?
                        local_class_sizes[current_class] : curr_case_size;
            }
            inside_case = false;
        }

        void visitProgramStmt(Program* stmt) { 
            for (auto& class_: stmt->classes) {
                class_->accept(this);
            }
        }

        void visitClassStmt(Class* stmt) { 
            current_class = stmt->name.lexeme;
            local_class_sizes.insert({current_class, 0});
            for (auto& f: stmt->features) {
                f->accept(this);
            }
        }

    private:
        std::unordered_map<std::string, size_t> local_func_sizes{};
        std::unordered_map<std::string, size_t> local_class_sizes{};
        std::string current_func;
        std::string current_class;
        bool inside_func;
        size_t curr_case_size{0};
        bool inside_case{false}; // to handle nested cases construction.

};

}