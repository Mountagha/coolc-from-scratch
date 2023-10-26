#include "ASTPrinter.hpp"

namespace cool {

void ASTPrinter::visitFeatureExpr(Feature* expr) {} 
void ASTPrinter::visitFormalExpr(Formal* expr)  {}
void ASTPrinter::visitAssignExpr(Assign* expr) {}
void ASTPrinter::visitIfExpr(If* expr) {}
void ASTPrinter::visitWhileExpr(While* expr) {}
void ASTPrinter::visitBinaryExpr(Binary* expr) {}
void ASTPrinter::visitUnaryExpr(Unary* expr) {}
void ASTPrinter::visitVariableExpr(Variable* expr) {}
void ASTPrinter::visitCallExpr(Call* expr) {}
void ASTPrinter::visitBlockExpr(Block* expr) {}
void ASTPrinter::visitGroupingExpr(Grouping* expr) {}
void ASTPrinter::visitGetExpr(Get* expr) {}
void ASTPrinter::visitLiteralExpr(Literal* expr) {}
void ASTPrinter::visitLetExpr(Let* expr) {}
void ASTPrinter::visitCaseExpr(Case* expr) {}


void visitProgramStmt(Program* stmt) {}
void visitClassStmt(Class* smtt) {}


}