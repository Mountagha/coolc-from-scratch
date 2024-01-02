#pragma once

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <tuple>

#include "token.hpp"
#include "utilities.hpp"
#include "environment.hpp"

namespace cool {


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

            // check every class in the program
            for (auto& class_ : stmt->classes) {
                curr_class = class_.get();
                symboltable.enterScope();
                class_->accept(this);
                symboltable.exitScope();
            } 
        }

        void visitClassStmt(Class* stmt) {
            // check every feature in each class.
            for (auto& feat: stmt->features) {
                feat->accept(this);
            }    
        }

        virtual void visitFeatureExpr(Feature* expr) {
            
            switch (expr->featuretype) {
                case FeatureType::ATTRIBUT:
                    check_attribut(expr);
                case FeatureType::METHOD:
                    check_method(expr);
            }
        }

        virtual void visitFormalExpr(Formal* expr) {
            if (symboltable.probe(expr->id.lexeme)) {
                throw std::runtime_error("Supplicated name.");
            }
            if (expr->id == self) {
                throw std::runtime_error("Can't use keyword 'self'. Preserved");
            }
            if (expr->type_ == SELF_TYPE) {
                throw std::runtime_error("Can't use the keyword 'SELF_TYPE'. Preserved.");
            }
            symboltable.insert(expr->id.lexeme, &expr->type_);
        }

        virtual void visitAssignExpr(Assign* expr) {
            expr->expr->accept(this);
            Token assign_type = expr->expr->expr_type;
            Token *id_type = symboltable.get(expr->id.lexeme);
            if (!id_type) {
                Class* target_class = curr_class;
                while (true) {
                    Feature* attr = get_feature(target_class, expr->id.lexeme, FeatureType::ATTRIBUT);
                    if (attr) {
                        *id_type = attr->expr_type;
                        break;       
                    }
                    Token parent = target_class->superClass->name;
                    if (parent == No_class)
                        break;
                    target_class = classTable.get(parent.lexeme);
                }
            }
            if (!id_type) {
                throw std::runtime_error("type error in object class");
            }
            if (!g.conform(assign_type, *id_type)) {
                throw std::runtime_error("type error in assign construct.");
            }
            expr->expr_type = assign_type;
        }

        virtual void visitIfExpr(If* expr) {
            expr->cond->accept(this);
            Token cond_type = expr->cond->expr_type;
            if (cond_type != Bool) {
                throw std::runtime_error("predicate inside If branch must have Bool type.");
            }
            expr->thenBranch->accept(this);
            expr->elseBranch->accept(this);
            Token join_type = g.lca(expr->thenBranch->expr_type, expr->elseBranch->expr_type);
            expr->expr_type = join_type;
        }

        virtual void visitWhileExpr(While* expr) {
            expr->cond->accept(this);
            if (expr->cond->expr_type != Bool) {
                throw std::runtime_error("predicate inside while branch must have Bool type.");
            }
            expr->expr->accept(this);
            expr->expr_type = Object;
        }

        virtual void visitBinaryExpr(Binary* expr) {

            expr->lhs->accept(this);
            expr->rhs->accept(this);
            switch (expr->op.token_type) {
                case PLUS:
                case MINUS:
                case STAR:
                case SLASH: {
                    if (expr->lhs->expr_type != Int || expr->rhs->expr_type != Int) {
                        expr->expr_type = Object;
                        throw std::runtime_error("Binary arithmetic operands must be type Int.");
                    }
                    expr->expr_type = Int;
                    break;
                }
                case LESS:
                case LESS_EQUAL: {
                    if (expr->lhs->expr_type != Int || expr->rhs->expr_type != Int) {
                        expr->expr_type = Object;
                        throw std::runtime_error("Binary comparison operands must be type Int.");
                    }
                    expr->expr_type = Bool;
                    break;
                }
                case EQUAL: {
                    Token lhs_type = expr->lhs->expr_type;
                    Token rhs_type = expr->rhs->expr_type;
                    if (lhs_type == Int) {
                        if (rhs_type == Int) {
                            expr->expr_type = Bool;
                        } else {
                            expr->expr_type = Object;
                            throw std::runtime_error("type mismatch in '=' operator.");
                        }
                    } else if (lhs_type == Bool) {
                        if (rhs_type == Bool) {
                            expr->expr_type = Bool;
                        } else {
                            expr->expr_type = Object;
                            throw std::runtime_error("type mismatch in '=' operator.");
                        }
                    } else if (lhs_type == Str) {
                        if (rhs_type == Str) {
                            expr->expr_type = Bool;
                        } else {
                            expr->expr_type = Object;
                            throw std::runtime_error("type mismatch in '=' operator.");
                        }
                    }
                    expr->expr_type = Bool;     // !TODO: recheck =. Not sure on the logics.
                    break;
                }

            }
        }

        virtual void visitUnaryExpr(Unary* expr) {
            expr->expr->accept(this);
            switch (expr->op.token_type) {
                case TILDE: {
                    if (expr->expr->expr_type != Int) {
                        expr->expr_type = Object;
                        throw std::runtime_error("Type error in '~' operator.");
                    }
                    expr->expr_type = Int;
                    break;
                }
                case NOT: {
                    if (expr->expr->expr_type != Bool) {
                        expr->expr_type = Object;
                        throw std::runtime_error("Type error in '=' operator.");
                    }
                    expr->expr_type = Bool;
                    break;
                }
                case ISVOID: {
                    expr->expr_type = Bool;
                    break;
                }
            }
        }

        virtual void visitVariableExpr(Variable* expr) {}
        virtual void visitCallExpr(Call* expr) {}

        virtual void visitBlockExpr(Block* expr) {
            Token block_type;
            for (auto& expr_: expr->exprs) {
                expr_->accept(this);
                block_type = expr_->expr_type;
            }
            expr->expr_type = block_type;
        }

        virtual void visitGroupingExpr(Grouping* expr) {}
        virtual void visitGetExpr(Get* expression) {}
        virtual void visitLiteralExpr(Literal* expr) {}

        virtual void visitLetExpr(Let* expr) {

            symboltable.enterScope();
            for (auto& let: expr->vecAssigns) {
                Token id = std::get<0>(let);
                Token id_type = std::get<1>(let);
                Expr* let_expr = std::get<2>(let).get(); // Since smart pointer.

                if (id == self) {
                    throw std::runtime_error("type error in let.");
                }

                if (let_expr) {
                    let_expr->accept(this);
                    if (let_expr->expr_type != No_type && !g.conform(id_type, let_expr->expr_type))
                        throw std::runtime_error("Type error in let assign");
                    symboltable.insert(id.lexeme, &let_expr->expr_type);
                } else {
                    symboltable.insert(id.lexeme, &std::get<1>(let)); // !TODO: doubt on pointer here.
                }
            }

            expr->body->accept(this);
            expr->expr_type = expr->body->expr_type;
            symboltable.exitScope();
        }

        virtual void visitCaseExpr(Case* expr) {

            Token join_type = Object;
            expr->expr->accept(this);
            Token expr0_type = expr->expr->expr_type;
            if (expr0_type == No_type)
                throw std::runtime_error("Type error in case expression.");
            
            SymbolTable<std::string, Token> casetable; // to track duplicated branches.
            for (auto& match: expr->matches) {
                symboltable.enterScope();

                Token id = std::get<0>(match);
                Token id_type = std::get<1>(match);
                Expr* match_expr = std::get<2>(match).get();

                if (casetable.get(id_type.lexeme)) {
                    throw std::runtime_error("Duplicated branch in case statement.");
                }

                casetable.insert(id_type.lexeme, &std::get<1>(match));

                match_expr->accept(this);

                if (!g.conform(match_expr->expr_type, id_type)) {
                    throw std::runtime_error("Type error in case evaluation.");
                }

                join_type = g.lca(join_type, match_expr->expr_type);

            }
            expr->expr_type = join_type;
        }

        void check_attribut(Feature* expr) {
            Token parent;
            Class* target_class;
            Feature* feat;

            if (expr->type_ == SELF_TYPE) {
                expr->type_ = curr_class->name;
            }

            if (expr->id.lexeme == "self"){
                throw std::runtime_error("Can't use keyword 'self' as name");
            }

            // ensure there's no attribute override.
            target_class = classTable.get(curr_class->superClass->name.lexeme);
            while (true) {
                feat = get_feature(curr_class, expr->id.lexeme, FeatureType::ATTRIBUT);
                if (feat) {
                    throw std::runtime_error("override occurs");
                    break;
                } 
                parent = target_class->superClass->name;
                if (parent == No_class)
                    break;
                target_class = classTable.get(parent.lexeme);
            }

            expr->expr->accept(this);       // check the init.

            Token init_type = expr->type_;
            if (init_type != No_type && !g.conform(init_type, expr->expr_type)) {
                throw std::runtime_error("type error in attr_class.");
            }
            symboltable.insert(expr->id.lexeme, &expr->type_);
        }

        void check_method(Feature* expr) {
            Feature* feat;
            Class* target_class;
            Token parent;            

            symboltable.enterScope();

            if (!classTable.get(expr->type_.lexeme) && expr->type_ != SELF_TYPE) {
                throw std::runtime_error("Invalid return type");
            }

            feat = nullptr;
            target_class = classTable.get(curr_class->name.lexeme);

            while (true) {
                feat = get_feature(target_class, expr->id.lexeme, FeatureType::METHOD);
                if (feat) 
                    break;
                parent = target_class->superClass->name;
                if (parent == No_class)
                    break;
                target_class = classTable.get(parent.lexeme);
            }

            if (feat) {
                if (feat->formals.size() != expr->formals.size()) {
                    throw std::runtime_error("Invalid formals.");
                }

                for (size_t i = 0; i < expr->formals.size(); i++) {
                    expr->formals[i]->accept(this);

                    if (expr->formals[i]->type_ != feat->formals[i]->type_) {
                        throw std::runtime_error("formal type mismatch");
                    }
                }
                if (expr->type_ != feat->type_) {
                    throw std::runtime_error("Formal type mismatch");
                }

            } else {
                for (auto& formal: expr->formals) {
                    formal->accept(this);
                }
            } 

            // check the body of the method.
            expr->expr->accept(this);

            // method return type must conform to body expr type.
            if (!g.conform(expr->type_, expr->expr->expr_type)) {
                throw std::runtime_error("Types do not conform.");
            }

            symboltable.exitScope();

        }

        Feature* get_feature(Class* stmt, const std::string& name, FeatureType ft) {
            for(auto& feat: stmt->features) {
                if (feat->id.lexeme == name && feat->featuretype == ft)
                    return feat.get();
            }
            return nullptr;
        }




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
        SymbolTable<std::string, Token> symboltable;
        unsigned int semant_errors;
        std::ostream& error_stream;
        bool class_main_exist{false};
        InheritanceGraph g;

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