#include "ASTPrinter.hpp"

namespace cool {

void ASTPrinter::visitFeatureExpr(Feature* expr) {
    ast_string += "Feature " + expr->id.lexeme + " (";
    ast_string.nl().indent();
    ast_string += "Type : " + expr->type_.lexeme + ",\n";
    ast_string += "Formals (";
    ast_string.nl().indent();
    for (auto& f: expr->formals)
        f->accept(this);
    ast_string.nl().unindent();
    ast_string += ")\n";
    ast_string += " BodyExprOrAssign : {";
    if (expr->expr != nullptr) {
        ast_string.nl().indent();
        expr->expr->accept(this);
        ast_string.nl().unindent();
    }
    ast_string += "}\n";
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
    ast_string.nl().unindent();
    ast_string += ")\n";
} 

void ASTPrinter::visitFormalExpr(Formal* expr)  {
    ast_string += "Formal (";
    ast_string.nl().indent();
    ast_string += "ID: " + expr->id.lexeme;
    ast_string.nl() += "Type: " + expr->type_.lexeme;
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitAssignExpr(Assign* expr) {
    ast_string.nl().indent();
    ast_string += expr->id.lexeme + " <- ";
    expr->expr->accept(this);
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
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
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
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
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
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
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
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
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitVariableExpr(Variable* expr) {
    ast_string += expr->name.lexeme;
    if (expr->expr_type) {
        ast_string += " [ ";
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
        ast_string += " ] ";
    }
 
}

void ASTPrinter::visitNewExpr(New* expr) {
    ast_string += "NEW ";
    ast_string += expr->type_.lexeme;
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
 
}

void ASTPrinter::visitBlockExpr(Block* expr) {
    ast_string += " Block (";
    ast_string.nl().indent();
    for (auto& e: expr->exprs) {
        e->accept(this);
        ast_string += "\n";
    }
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitGroupingExpr(Grouping* expr) {
    ast_string += "Grouping (";
    ast_string.nl().indent();
    expr->expr->accept(this);
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitStaticDispatchExpr(StaticDispatch* expr) {
    ast_string += "static_dispatch (";
    ast_string.nl().indent();
    ast_string += "callee name: ";
    ast_string += expr->callee_name.lexeme;
    ast_string.nl();
    ast_string += "expr: ";
    expr->expr->accept(this);
    ast_string.nl();
    ast_string += "class: ";
    ast_string += expr->class_.lexeme;
    ast_string.nl();
    ast_string += "Args ( ";
    // ast_string.nl().indent();
    for (auto& arg: expr->args) {
        arg->accept(this);
        ast_string += ", ";
    }
    ast_string += ")\n";
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitDispatchExpr(Dispatch* expr) {
    ast_string += "dynamic_dispatch (";
    ast_string.nl().indent();
    ast_string += "callee name: ";
    ast_string += expr->callee_name.lexeme;
    ast_string.nl();
    ast_string += "expr: ";
    expr->expr->accept(this);
    ast_string.nl();
    ast_string += "Args ( ";
    //ast_string.nl().indent();
    for (auto& arg: expr->args) {
        arg->accept(this);
        ast_string += ", ";
    }
    ast_string += ")\n";
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitLiteralExpr(Literal* expr) {
    ast_string += expr->object.to_string();
    if (expr->expr_type) {
        ast_string += " [ ";
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
        ast_string += " ] ";
    }
    
}

void ASTPrinter::visitLetExpr(Let* expr) {
    ast_string += "let (";
    ast_string.nl().indent();
    for (auto& assign: expr->vecAssigns) {
        auto formal = std::get<0>(assign).get();
        auto expr1 = std::get<1>(assign).get();
        formal->accept(this);
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
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
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
        auto formal = std::get<0>(match).get();
        auto expr1 = std::get<1>(match).get();
        formal->accept(this);
        ast_string += " => ";
        expr1->accept(this);
        ast_string.nl();
    }
    ast_string.nl().unindent();
    ast_string += "\n)";
    if (expr->expr_type)
        ast_string += "infered TYPE : " + expr->expr_type.lexeme;
    ast_string.nl().unindent();
    ast_string += "\n)";
}


void ASTPrinter::visitProgramStmt(Program* stmt) {
    ast_string += "Program (";
    ast_string.nl().indent();
    for (auto& c : stmt->classes) {
        c->accept(this);
    }
    ast_string.nl().unindent();
    ast_string += ")\n";
}

void ASTPrinter::visitClassStmt(Class* stmt) {
    ast_string += "Class " + stmt->name.lexeme + " "; 
    ast_string += ": SUPERCLASS " + stmt->superClass.lexeme;
    ast_string.nl().indent();
    for (auto& f : stmt->features) {
        f->accept(this);
    }
    ast_string.nl().unindent();
    ast_string += ")\n";
}


} // namespace cool