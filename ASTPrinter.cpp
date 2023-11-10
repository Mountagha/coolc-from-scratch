#include "ASTPrinter.hpp"

namespace cool {

void ASTPrinter::visitFeatureExpr(Feature* expr) {
    ast_string += "(Feature " + expr->id.lexeme + " (";
    ast_string.nl().indent();
    ast_string += "Type : " + expr->type_.lexeme + ",";
    if (expr->formals.size() > 0) {
        ast_string.nl().indent();
        ast_string += "(Formals (";
        for (auto& f: expr->formals) 
            f->accept(this);
        ast_string.nl().unindent();
        ast_string += " Expr : {";
        ast_string.nl().indent();
        expr->expr->accept(this);
        ast_string.unindent();
    } else {
        if (expr->expr != nullptr) {
            ast_string.nl();
            ast_string += "Assign ( ";
            ast_string.nl().indent();
            expr->expr->accept(this);
            ast_string.nl().unindent();
            ast_string += ")";
        }
    }
} 

void ASTPrinter::visitFormalExpr(Formal* expr)  {
    ast_string.nl().indent(); 
    ast_string += "Formal (";
    ast_string.nl().indent();
    ast_string += "ID: " + expr->id.lexeme;
    ast_string.nl() += "Type: " + expr->type_.lexeme;
    ast_string.nl().unindent();
    ast_string += ")";
}

void ASTPrinter::visitAssignExpr(Assign* expr) {
    ast_string.nl().indent();
    ast_string += expr->id.lexeme + " <- ";
    expr->expr->accept(this);
    ast_string.unindent();
}

void ASTPrinter::visitIfExpr(If* expr) {
    ast_string += "If (";
    ast_string.nl().indent();
    ast_string += "Cond (";
    ast_string.nl().indent();
    expr->cond->accept(this);
    ast_string.nl().unindent();
    ast_string += ")\n";
    ast_string += "ThenBranch (";
    ast_string.nl().indent();
    expr->thenBranch->accept(this);
    ast_string.nl().unindent();
    ast_string += ")\n";
    ast_string += "ElseBranch ( ";
    ast_string.nl().indent();
    expr->elseBranch->accept(this);
    ast_string.nl().unindent();
    ast_string += ")";
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitWhileExpr(While* expr) {
    ast_string += " WhileLoop (";
    ast_string.nl().indent();
    ast_string += "Cond (";
    ast_string.nl().indent();
    expr->cond->accept(this);
    ast_string.nl().unindent();
    ast_string += ")\n";
    ast_string += "Body (";
    ast_string.nl().indent();
    expr->expr->accept(this);
    ast_string.nl().unindent();
    ast_string += ")";
    ast_string.nl().unindent();
    ast_string += ")";
}

void ASTPrinter::visitBinaryExpr(Binary* expr) {
    ast_string += "BinaryOp (";
    ast_string.nl().indent();
    ast_string += expr->op.lexeme + "\n";
    ast_string += "LHS (";
    expr->lhs->accept(this);
    ast_string += ")\n";
    ast_string += "RHS (";
    expr->rhs->accept(this);
    ast_string += ")";
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitUnaryExpr(Unary* expr) {
    ast_string += "UnaryOp (";
    ast_string.nl().indent();
    ast_string += expr->op.lexeme + "\n";
    ast_string += "Expr (";
    expr->expr->accept(this);
    ast_string += ")";
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitVariableExpr(Variable* expr) {
    ast_string += expr->name.lexeme;
}

void ASTPrinter::visitCallExpr(Call* expr) {
    ast_string += "Call (";
    ast_string.nl().indent();
    ast_string += "Callee (";
    expr->callee->accept(this);
    ast_string += ")\n";
    ast_string += "Args ( ";
    for (auto& arg: expr->args) {
        arg->accept(this);
    }
    ast_string += ")";
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitBlockExpr(Block* expr) {
    ast_string += " Block (";
    ast_string.nl().indent();
    for (auto& e: expr->exprs) {
        e->accept(this);
    }
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitGroupingExpr(Grouping* expr) {
    ast_string += "Grouping (";
    ast_string.nl().indent();
    expr->expr->accept(this);
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitGetExpr(Get* expr) {
    ast_string += "Get (";
    ast_string.nl().indent();
    expr->expr->accept(this);
    if (expr->class_ != nullptr) {
        ast_string += "@";
        expr->class_->accept(this);
    }
    ast_string.nl().unindent();
    ast_string += ")\n";
    
}

void ASTPrinter::visitLiteralExpr(Literal* expr) {
    ast_string += expr->object.to_string();
}

void ASTPrinter::visitLetExpr(Let* expr) {
    ast_string += "let (";
    ast_string.nl().indent();
    for (auto& assign: expr->vecAssigns) {
        auto id = std::get<0>(assign);
        auto type_ = std::get<1>(assign);
        auto expr1 = std::move(std::get<2>(assign));
        ast_string += id.lexeme + " : " + type_.lexeme;
        if (expr1 != nullptr) {
            ast_string += " <- ";
            expr1->accept(this);
        }
        ast_string.nl();
    }
    ast_string += "body (";
    ast_string.nl().indent();
    expr->body->accept(this);
    ast_string.nl().unindent();
    ast_string += ")\n";
    ast_string.unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitCaseExpr(Case* expr) {
    ast_string += "case (";
    ast_string.nl().indent();
    expr->expr->accept(this);
    ast_string += "body: ";
    ast_string.nl().indent();
    for (auto& match : expr->matches){
        auto id = std::get<0>(match);
        auto type_ = std::get<1>(match);
        auto expr1 = std::move(std::get<2>(match));
        ast_string += id.lexeme + " : " + type_.lexeme + " => ";
        expr1->accept(this);
        ast_string.nl();
    }
    ast_string.nl().unindent();
    ast_string += "\n)";
    ast_string.nl().unindent();
    ast_string += "\n)";
}


void ASTPrinter::visitProgramStmt(Program* stmt) {
    ast_string += "Program (";
    ast_string.nl().indent();
    for (auto& c : stmt->classes) {
        c->accept(this);
    }
    ast_string.unindent();
    ast_string.nl() += ")";
}

void ASTPrinter::visitClassStmt(Class* stmt) {
    ast_string += "Class " + stmt->name.lexeme + " "; 
    if (stmt->superClass != nullptr)
        ast_string += ": SUPERCLASS " + stmt->superClass->name.lexeme;
    ast_string.nl().indent();
    for (auto& f : stmt->features) {
        f->accept(this);
    }
    ast_string.nl().unindent();
    ast_string += ")\n";
}


}