/* A pretty printer of the ast generated by the parser. */
#pragma once

#include "ast.hpp"
#include <iostream>
#include <ostream>

namespace cool {

class ASTPrinter : public ExprVisitor, public StmtVisitor  {
    public:
        ASTPrinter() = default;

        void print(std::unique_ptr<Expr>& expr) {
            expr->accept(this);
            std::cout << ast_string;
        }

        void print(std::unique_ptr<Stmt>& stmt) {
            stmt->accept(this);
            std::cout << ast_string;
        }

        void visitFeatureExpr(Feature* expr);
        void visitFormalExpr(Formal* expr);
        void visitAssignExpr(Assign* expr);
        void visitIfExpr(If* expr);
        void visitWhileExpr(While* expr);
        void visitBinaryExpr(Binary* expr);
        void visitUnaryExpr(Unary* expr);
        void visitVariableExpr(Variable* expr); 
        void visitCallExpr(Call* expr);
        void visitBlockExpr(Block* expr);
        void visitGroupingExpr(Grouping* expr);
        void visitGetExpr(Get* expr);
        void visitLiteralExpr(Literal* expr);
        void visitLetExpr(Let* expr);
        void visitCaseExpr(Case* expr);


        void visitProgramStmt(Program* stmt);
        void visitClassStmt(Class* smtt);

    private:
        class PrettyString {
            private:
                std::string s;
                unsigned int l;
            public:
                PrettyString(): s{""}, l{0} {}
                ~ PrettyString() = default;

                PrettyString& operator+=(std::string other) {
                    if (s.back() == '\n')
                        for (auto i=0; i<l; i++) s += "\t";
                    s += other;
                    return *this;
                }

                PrettyString& operator+=(const char* other) {
                    if (s.back() == '\n')
                        for (auto i=0; i<l; i++) s += "\t";
                    s += other;
                    return *this;
                }

                PrettyString& operator+(std::string other) {
                    *this += other;
                    return *this;
                }

                friend std::ostream& operator<<(std::ostream& os, const PrettyString& o) {
                    os << o.s << "\n";
                    return os;
                }
                PrettyString& nl() { s += "\n"; return *this; }
                void indent() { l++; }
                void unindent() { l--; }

        };
        PrettyString ast_string;
};

} // namespace cool