#pragma once

#include <iostream>
#include <ostream>

#include "token.hpp"
#include "utilities.hpp"
#include "localsizer.hpp"
#include "ast.hpp"
#include "environment.hpp"
#include "constants.hpp"

namespace cool {

class Cgen: public StmtVisitor, public ExprVisitor {

    public:
        Cgen(InheritanceGraph* g_, SymbolTable<std::string, Class* >* ctable_ptr, std::ostream& out=std::cout): 
            os{out}, class_table_ptr(ctable_ptr), g(g_), curr_attr_count{0}, ifcount{0}, while_count{0}, casecount{0}, dispatch_count{0} {
                
            classtag_map.insert({"Bool", BOOL_CLASS_TAG});
            classtag_map.insert({"String", STRING_CLASS_TAG});
            classtag_map.insert({"Int", INT_CLASS_TAG});
        }

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
        InheritanceGraph* g; // from semantic analyzer. 
        SymbolTable<std::string, Class* >* class_table_ptr;
        Class* curr_class;

        // contains mapping of [class_name][method_name] -> offset in 
        // dispatch table used to implement dispatch.
        std::map<std::string, std::map<std::string, int>> method_table;       

        // table of class attributes used to detemine valid names 
        // that are in scope
        std::map<std::string, std::map<std::string, int>> attr_table;       

        // Used to keep track of current attributes counts for a specific
        // class when generating code for class_init methods.
        std::size_t curr_attr_count;

        // Used to generate labels for ifs
        std::size_t ifcount;

        // Used to generate labels for whiles
        std::size_t while_count;

        // Used to track frame pointer offset for local variables in functions.
        std::size_t fp_offset;

        // Used to track frame pointer offset for local variables in Class(Init_functions of clases)
        std::size_t class_fp_offset;

        // Used to track case branches.
        std::size_t casecount;

        // Used to track in and out of functions
        bool inside_function{false};

        // Used to track dispatch labels
        std::size_t dispatch_count;

        std::unordered_map<std::string, int> classtag_map{};

        // The variable environment that maps variable names to offsets
        // in the current AR relative to the fp. this allows for easier
        // addressing. eg. the first parameter is in 4($fp), next is 8($fp)... n($fp)
        SymbolTable<std::string, int> var_env;

        // The localSizer is a pass that compute the size of locals in every function
        LocalSizer localsizer{};

        /*
            Class tags for some basic classes
            Note that there are no pre-defined class tags for class Object
            and IO simply because there is no need. These class tags are
            used for determining equality since an object of type String
            can only be compared with another object of the same type. The same
            goes for Int and Bool.
        */

        const int OBJECT_CLASS_TAG = 4; // lowest class Tag. The choices here are completely random.
        const int BOOL_CLASS_TAG = 5;
        const int INT_CLASS_TAG = 6;
        const int STRING_CLASS_TAG = 7;

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
        void emit_not(const char* );
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
        void emit_bgt(const char*, int, const std::string&);
        void emit_blt(const char*, int, const std::string&);
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
        void emit_pop(const char* );

        // push the content of a register onto the stack
        void emit_push(const char*);

        void emit_protobj_ref(const char*);
        void emit_init_ref(const char*);

        // emit code for each object's dispatch table
        void code_dispatch_table(Class*);

        // emit code for prototype objects
        void code_prototype_objects();

        // calculates the size of an object, used when generating
        // code for each object prototype.
        int calc_obj_size(Class* );

        // emit code for the prototype objects attributes
        void emit_obj_attributes(Class* );

        // debugging purposes
        void emit_comment(const std::string& );

        // emit code for Garbage collector functions choice.
        void code_select_gc();

        // emit code to start the .text segment and to 
        // declare the global names.
        void code_global_text();

        // emit code to declare global names.
        void code_global_data();

        void class_name_table();

        void cgen_method(Feature* );

        void cgen_attribut(Feature* );

        void cgen_init_formal(Token& );

        // this method will attribute to each class a tag 
        // which will be used to compare classes (case construct)
        // and also do some indexing (classNameTab)
        void construct_classtag_map();

        // help emitting strings that contains chars that got us in trouble.
        void print_string_literal(const std::string& s);
        void print_string_in_bytes(const std::string& s);


};

} // namespace cool