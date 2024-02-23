#pragma once

#include <iostream>
#include <ostream>

#include "token.hpp"
//#include "utilities.hpp"
#include "ast.hpp"
#include "environment.hpp"

namespace cool {

class Cgen: public StmtVisitor, public ExprVisitor {

    public:
        Cgen(std::ostream& out=std::cout): os{out} {}

        void cgen(std::unique_ptr<Expr>& expr) {
            expr->accept(this);
        }

        void cgen(std::unique_ptr<Stmt>& stmt) {
            stmt->accept(this);
        }

        // emit code for string and integer constants
        void code_constants();

        void visitFeatureExpr(Feature* expr);
        void visitFormalExpr(Formal* expr);
        void visitAssignExpr(Assign* expr);
        void visitIfExpr(If* expr);
        void visitWhileExpr(While* expr);
        void visitBinaryExpr(Binary* expr);
        void visitUnaryExpr(Unary* expr);
        void visitVariableExpr(Variable* expr);
        void visitNewExpr(New* expr);
        void visitBlockExpr(Block* expr);
        void visitGroupingExpr(Grouping* expr);
        void visitStaticDispatchExpr(StaticDispatch* expr);
        void visitDispatchExpr(Dispatch* expr);
        void visitLiteralExpr(Literal* expr);
        void visitLetExpr(Let* expr);
        void visitCaseExpr(Case* expr);
        void visitProgramStmt(Program* stmt);
        void visitClassStmt(Class* stmt);


    private:
        std::ostream& os;

        //////////////////////////////////////////////////////////////////////////////
        //
        //  emit_* procedures
        //
        //  emit_X  writes code for operation "X" to the output stream.
        //  There is an emit_X for each opcode X, as well as emit_ functions
        //  for generating names according to the naming conventions (see emit.h)
        //  and calls to support functions defined in the trap handler.
        //
        //  Register names and addresses are passed as strings.  See `emit.h'
        //  for symbolic names you can use to refer to the strings.
        //
        //////////////////////////////////////////////////////////////////////////////

        // generic instructions
        void emit_align(int);
        void emit_ascii(const std::string&);
        void emit_asciiz(const std::string&);
        void emit_byte(int);
        void emit_global(const std::string&&);
        void emit_word(int);
        void emit_word(const std::string&);
        void emit_label(const std::string&);

        // arithmetic instructions.
        void emit_add(const char*, const char*, const char*);
        void emit_addu(const char*, const char*, const char*);
        void emit_addi(const char*, const char*, int);
        void emit_addiu(const char*, const char*, int);
        void emit_div(const char*, const char*, const char*);
        void emit_divu(const char*, const char*, const char*);
        void emit_mul(const char*, const char*, const char*);
        void emit_sub(const char*, const char*, const char*);

        // logical instructions
        void emit_and(const char*, const char*, const char*);
        void emit_neg(const char*, const char*);
        void emit_nor(const char*, const char*, const char*);
        void emit_not(const char*, const char*);
        void emit_or(const char*, const char*, const char*);
        void emit_xor(const char*, const char*, const char*);

        // constant manipulating instructions
        void emit_li(const char*, int);
        void emit_lui(const char*, int);

        // comparison instructions
        void emit_seq(const char*, const char*, const char*);
        void emit_seq(const char*, const char*, int);
        void emit_sge(const char*, const char*, const char*);
        void emit_sge(const char*, const char*, int);
        void emit_sgt(const char*, const char*, const char*);
        void emit_sgt(const char*, const char*, int);
        void emit_sle(const char*, const char*, const char*);
        void emit_sle(const char*, const char*, int);
        void emit_sne(const char*, const char*, const char*);
        void emit_sne(const char*, const char*, int);

        // branch and jump instructions
        void emit_b(const std::string&);
        void emit_beq(const char*, const char*, const std::string&);
        void emit_beq(const char*, int, const std::string&);
        void emit_bge(const char*, const char*, const std::string&);
        void emit_bge(const char*, int, const std::string&);
        void emit_bne(const char*, const char*, const std::string&);
        void emit_bne(const char*, int, const std::string&);
        void emit_j(const std::string&);
        void emit_jal(const std::string&);
        void emit_jalr(const char*);
        void emit_jr(const char*);

        // load instructions
        void emit_la(const char*, const std::string&);
        void emit_lb(const char*, const char*);
        void emit_ld(const char*, const char*);
        void emit_lw(const char*, int, const char*);

        // store instructions
        void emit_sb(const char*, const char*);
        void emit_sd(const char*, const char*);
        void emit_sw(const char*, int, const char*);

        // data movement instructions
        void emit_move(const char*, const char*);

        // stack operations
        // not that these functions take the number of 32-bit words to push
        void emit_push(int);
        void emit_pop(int);





};

}