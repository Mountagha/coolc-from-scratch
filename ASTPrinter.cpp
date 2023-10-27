#include "ASTPrinter.hpp"

namespace cool {

void ASTPrinter::visitFeatureExpr(Feature* expr) {
    ast_string += expr->id.lexeme;
    if (expr->formals.size() > 0) {
        ast_string += " (\n";
        for (auto& f: expr->formals) 
            f->accept(this);
        ast_string += " )\n";
        ast_string += " : " + expr->type_.lexeme + "{ ";
        expr->expr->accept(this);
        ast_string += " }";
    } else {
        ast_string += " : " + expr->type_.lexeme;
        if (expr->expr != nullptr) {
            ast_string += " <- ";
            expr->expr->accept(this);
        }
    }
} 

void ASTPrinter::visitFormalExpr(Formal* expr)  {
    ast_string += expr->id.lexeme + " : " + expr->type_.lexeme;
}

void ASTPrinter::visitAssignExpr(Assign* expr) {
    ast_string += expr->id.lexeme + " <- ";
    expr->expr->accept(this);
}

void ASTPrinter::visitIfExpr(If* expr) {
    ast_string += "if ";
    expr->cond->accept(this);
    ast_string += "then ";
    expr->thenBranch->accept(this);
    ast_string += "else ";
    expr->elseBranch->accept(this);
    ast_string += " fi\n";
}

void ASTPrinter::visitWhileExpr(While* expr) {
    ast_string += " while ";
    expr->cond->accept(this);
    ast_string += " loop ";
    expr->expr->accept(this);
    ast_string += " pool\n";
}
void ASTPrinter::visitBinaryExpr(Binary* expr) {
    expr->lhs->accept(this);
    ast_string += expr->op.lexeme;
    expr->rhs->accept(this);
}

void ASTPrinter::visitUnaryExpr(Unary* expr) {
    ast_string += expr->op.lexeme;
    expr->expr->accept(this);
}

void ASTPrinter::visitVariableExpr(Variable* expr) {
    ast_string += expr->name.lexeme;
}

void ASTPrinter::visitCallExpr(Call* expr) {
    expr->callee->accept(this);
    ast_string += "(";
    for (auto& arg: expr->args) {
        arg->accept(this);
        ast_string += ", ";
    }
    ast_string += ")";
}

void ASTPrinter::visitBlockExpr(Block* expr) {
    ast_string += " {\n";
    for (auto& e: expr->exprs) {
        e->accept(this);
        ast_string += ";";
    }
    ast_string += " }\n";
}
void ASTPrinter::visitGroupingExpr(Grouping* expr) {
    ast_string += " (\n";
    expr->expr->accept(this);
    ast_string += " )\n";
}

void ASTPrinter::visitGetExpr(Get* expr) {
    expr->expr->accept(this);
    if (expr->class_ != nullptr) {
        ast_string += "@";
        expr->class_->accept(this);
    }
    ast_string += "." + expr->name.lexeme;
    
}

void ASTPrinter::visitLiteralExpr(Literal* expr) {
    ast_string += expr->object.to_string();
}

void ASTPrinter::visitLetExpr(Let* expr) {
    ast_string += "let ";
    for (auto& assign: expr->vecAssigns) {
        auto id = std::get<0>(assign);
        auto type_ = std::get<1>(assign);
        auto expr1 = std::move(std::get<2>(assign));
        ast_string += id.lexeme + " : " + type_.lexeme;
        if (expr1 != nullptr) {
            ast_string += " <- ";
            expr1->accept(this);
        }
    }
    ast_string += "in \n";
    expr->body->accept(this);
}

void ASTPrinter::visitCaseExpr(Case* expr) {
    ast_string += "case ";
    expr->expr->accept(this);
    ast_string += "of ";
    for (auto& match : expr->matches){
        auto id = std::get<0>(match);
        auto type_ = std::get<1>(match);
        auto expr1 = std::move(std::get<2>(match));
        ast_string += id.lexeme + " : " + type_.lexeme + " => ";
        expr1->accept(this);
    }
}


void ASTPrinter::visitProgramStmt(Program* stmt) {
    ast_string += "Program [\n";
    for (auto& c : stmt->classes) {
        c->accept(this);
    }
    ast_string += " ]";
}
void ASTPrinter::visitClassStmt(Class* stmt) {
    ast_string += "Class " + stmt->name.lexeme + " "; 
    if (stmt->superClass != nullptr)
        ast_string += "inherits " + stmt->superClass->name.lexeme;
    ast_string += " {\n";
    for (auto& f : stmt->features) {
        f->accept(this);
    }
    ast_string += " }";
}


}