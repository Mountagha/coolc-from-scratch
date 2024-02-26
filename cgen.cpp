#include <stack>

#include "cgen.hpp"
#include "emit.hpp"


namespace cool {

void Cgen::emit_add(const char* dest, const char* src1, const char* src2) {
    os << ADD << dest << ", $" << src1 << ", $" << src2 << std::endl; 
}

void Cgen::emit_addu(const char* dest, const char* src1, const char* src2) {
    os << ADDU << dest << ", $" << src1 << ", $" << src2 << std::endl; 
}

void Cgen::emit_addi(const char* dest, const char* src1, int imm) {
    os << ADDI << dest << ", $" << src1 << ", " << imm << std::endl; 
}

void Cgen::emit_addiu(const char* dest, const char* src1, int imm) {
    os << ADDIU << dest << ", $" << src1 << ", " << imm << std::endl; 
}

void Cgen::emit_div(const char* dest, const char* src1, const char* src2) {
    os << DIV << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

void Cgen::emit_divu(const char* dest, const char* src1, const char* src2) {
    os << DIVU << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

void Cgen::emit_mul(const char* dest, const char* src1, const char* src2) {
    os << MUL << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

void Cgen::emit_sub(const char* dest, const char* src1, const char* src2) {
    os << SUB << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

//

void Cgen::emit_and(const char* dest, const char* src1, const char* src2) {
    os << AND << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

void Cgen::emit_neg(const char* dest, const char* src) {
    os << NEG << dest << ", $" << src << std::endl;
}

void Cgen::emit_nor(const char* dest, const char* src1, const char* src2) {
    os << NOR << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

void Cgen::emit_not(const char* dest, const char* src) {
    os << NOT << dest << ", $" << src << std::endl;
}

void Cgen::emit_or(const char* dest, const char* src1, const char* src2) {
    os << OR << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

void Cgen::emit_xor(const char* dest, const char* src1, const char* src2) {
    os << XOR << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

//

void Cgen::emit_li(const char* dest, int imm) {
    os << LI << dest << ", " << imm << std::endl;
}

void Cgen::emit_lui(const char* dest, int imm) {
    os << LUI << dest << ", " << imm << std::endl;
}

//

void Cgen::emit_seq(const char* dest, const char* src1, const char* src2) {
    os << SEQ << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

void Cgen::emit_seq(const char* dest, const char* src1, int imm) {
    os << SEQ << dest << ", $" << src1 << ", " << imm << std::endl;
}

void Cgen::emit_sge(const char* dest, const char* src1, const char* src2) {
    os << SGE << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

void Cgen::emit_sge(const char* dest, const char* src1, int imm) {
    os << SGE << dest << ", $" << src1 << ", " << imm << std::endl;
}

void Cgen::emit_sgt(const char* dest, const char* src1, const char* src2) {
    os << SGE << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

void Cgen::emit_sgt(const char* dest, const char* src1, int imm) {
    os << SGE << dest << ", $" << src1 << ", " << imm << std::endl;
}

void Cgen::emit_sle(const char* dest, const char* src1, const char* src2) {
    os << SLE << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

void Cgen::emit_sle(const char* dest, const char* src1, int imm) {
    os << SLE << dest << ", $" << src1 << ", " << imm << std::endl;
}

void Cgen::emit_sne(const char* dest, const char* src1, const char* src2) {
    os << SNE << dest << ", $" << src1 << ", $" << src2 << std::endl;
}

void Cgen::emit_sne(const char* dest, const char* src1, int imm) {
    os << SNE << dest << ", $" << src1 << ", " << imm << std::endl;
}

void Cgen::emit_b(const std::string& label) {
    os << B << label << std::endl;
}

void Cgen::emit_beq(const char* src1, const char* src2, const std::string& label) {
    os << BEQ << src1 << ", $" << src2 << ", " << label << std::endl;
}

void Cgen::emit_beq(const char* src, int imm, const std::string& label) {
    os << BEQ << src << ", " << imm << ", " << label << std::endl;
}

void Cgen::emit_bge(const char* src1, const char* src2, const std::string& label) {
    os << BGE << src1 << ", $" << src2 << ", " << label << std::endl;
}

void Cgen::emit_bge(const char* src, int imm, const std::string& label) {
    os << BGE << src << ", " << imm << ", " << label << std::endl;
}

void Cgen::emit_bne(const char* src1, const char* src2, const std::string& label) {
    os << BNE << src1 << ", $" << src2 << ", " << label << std::endl;
}

void Cgen::emit_bne(const char* src, int imm, const std::string& label) {
    os << BNE << src << ", " << imm << ", " << label << std::endl;
}

void Cgen::emit_j(const std::string& label) {
    os << JUMP << label << std::endl;
}

void Cgen::emit_jal(const std::string& label) {
    os << JAL << label << std::endl;
}

void Cgen::emit_jalr(const char* src) {
    os << JALR << src << std::endl;
}

void Cgen::emit_jr(const char* src) {
    os << JR << src << std::endl;
}

//

void Cgen::emit_la(const char* dest, const std::string& addr) {
    os << LA << dest << ", " << addr << std::endl;
}

void Cgen::emit_lb(const char* dest, const char* addr) {
    os << LB << dest << ", " << addr << std::endl;
}

void Cgen::emit_ld(const char* dest, const char* addr) {
    os << LD << dest << ", " << addr << std::endl;
}

void Cgen::emit_lw(const char* dest, int offset, const char* src) {
    os << LW << dest << ", " << offset << "($" << src << ")" << std::endl;
}

void Cgen::emit_sb(const char* dest, const char* addr) {
    os << SB << dest << ", " << addr << std::endl;
}

void Cgen::emit_sw(const char* dest, int offset, const char* src) {
    os << SW << dest << ", " << offset << "($" << src << ")" << std::endl;
}

void Cgen::emit_move(const char* dest, const char* src) {
    os << MOVE << dest << ", $" << src << std::endl;
}

void Cgen::emit_ascii(const std::string& s) {
    os << "\t.ascii\t\"" << s << "\"\n";
}

void Cgen::emit_asciiz(const std::string& s) {
    os << "\t.asciiz\t\"" << s << "\"\n";
}


//********************************************************
//
// Emit code to reserve space for and initialize all of
// the constants.  Class names should have been added to
// the string table (in the supplied code, is is done
// during the construction of the inheritance graph), and
// code for emitting string constants as a side effect adds
// the string's length to the integer table.  The constants
// are emmitted by running through the stringtable and inttable
// and producing code for each entry.
//
//********************************************************


void Cgen::code_constants() {
    //
    // Add constants that are required by the code generator
    //
    // add class names to string constants.
    for (auto& class_: g.get_graph()) {
        stringtable().insert({class_.first.lexeme, class_.first});
    }
    
    // add empty string to string const table since it's the default value 
    // value of a newly allocated string.
    stringtable().insert({"", Token{TokenType::_NULL, ""}});

    int index = 1;
    for (auto& elt: stringtable()) {
        
        os << INTCONST_PREFIX << index << LABEL;                                                // label
        os << WORD << STRING_CLASS_TAG << std::endl;                                            // tag 
        os << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + (elt.first.size()/4)) << std::endl;   // size
        os << WORD << "String" << DISPTAB_SUFFIX << std::endl;
        os << WORD << elt.first.size() << std::endl;
        os << ASCIIZ << "\"" << elt.first.c_str() << "\"\n";
        os << ALIGN;
        index++;

    }

    index = 1;
    for (auto& elt: inttable()) {

        os << INTCONST_PREFIX << index << LABEL;                                                // label
        os << WORD << INT_CLASS_TAG << std::endl;
        os << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << std::endl;
        os << WORD << "Int" << DISPTAB_SUFFIX << std::endl;
        os << WORD << elt.first << std::endl;
        index++;

    }

    // code gen for bools
    os << BOOLCONST_PREFIX << "0" << LABEL; // false
    os << WORD << BOOL_CLASS_TAG << std::endl;
    os << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << std::endl; 
    os << WORD << "Bool" << DISPTAB_SUFFIX << std::endl;
    os << WORD << "0" << std::endl;

    os << BOOLCONST_PREFIX << "1" << LABEL; // true
    os << WORD << BOOL_CLASS_TAG << std::endl;
    os << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << std::endl; 
    os << WORD << "Bool" << DISPTAB_SUFFIX << std::endl;
    os << WORD << "1" << std::endl;

}

void Cgen::code_dispatch_table(Class* class_) {
    std::map<Token, Token>  mnames;
    std::stack<Class*> class_ordering;

    Class* curr_class = class_;
    
    while (curr_class->name != No_class) {
        class_ordering.push(curr_class);
        for(auto& m: curr_class->features) {
            if (m->featuretype == FeatureType::METHOD) 
                if (mnames.find(m->id) == mnames.end())
                    mnames.insert({m->id, curr_class->name});
        }
        curr_class = class_table_ptr->get(curr_class->superClass); 
    }

    int dispoffset = 0; 

    while (!class_ordering.empty()) {
        curr_class = class_ordering.top();

        for (auto& m: curr_class->features) {
            if (mnames.find(m->id) != mnames.end()) {
                method_table[curr_class->name][m->id] = dispoffset++;
                os << WORD << curr_class->name << METHOD_SEP << m->id << std::endl;
                mnames.erase(m->id);
            }
        }
        class_ordering.pop();
    }
}

int Cgen::calc_obj_size(Class* class_) {
    int total = 0;
    Class* curr_class = class_;

    while (curr_class->name != No_class) {
        for(auto& f: curr_class->features) {
            if (f->featuretype == FeatureType::ATTRIBUT) {
                total++;
            }
        }
        curr_class = class_table_ptr->get(curr_class->superClass);
    }
    
    return total;
}

void Cgen::emit_obj_attributes(Class* class_) {
    if (class_->name == No_class)
        return;

    emit_obj_attributes(class_table_ptr->get(class_->superClass));

    for (auto& f: class_->features)
        if (f->featuretype == FeatureType::ATTRIBUT)
            os << WORD << "0" << std::endl;
}


void Cgen::code_prototype_objects() {
    int classtag = 8;    // to avoid it being equal to basic class values
    for (auto& class_: g.get_graph()) {

        if (class_.first == No_class) 
            return;
        
        os << class_.first.lexeme << PROTOBJ_SUFFIX << LABEL;

        if (class_.first == Str)
            os << WORD << STRING_CLASS_TAG << std::endl;
        else if (class_.first == Int)
            os << WORD << INT_CLASS_TAG << std::endl;
        else if (class_.first == Bool)
            os << WORD << BOOL_CLASS_TAG << std::endl;
        else 
            os << WORD << classtag++ << std::endl;

        os << WORD << (DEFAULT_OBJFIELDS + calc_obj_size(class_table_ptr->get(class_.first)));
        os << WORD << class_.first.lexeme << DISPTAB_SUFFIX << std::endl;
        emit_obj_attributes(class_table_ptr->get(class_.first));
    }
}

void Cgen::code_global_data() {

    //os << "\t.data\n" << ALIGN;
    //   << 

}

// Cgen for Exprs and Stmts
void Cgen::visitProgramStmt(Program* stmt) {
    std::cout << "debut code generation\n\n";
    code_constants();
    std::cout << "fin code generation\n\n";
}


void Cgen::visitClassStmt(Class* stmt) {}

void Cgen::visitFeatureExpr(Feature* expr) {}
void Cgen::visitFormalExpr(Formal* expr) {}
void Cgen::visitAssignExpr(Assign* expr) {}
void Cgen::visitIfExpr(If* expr) {}
void Cgen::visitWhileExpr(While* expr) {}
void Cgen::visitBinaryExpr(Binary* expr) {}
void Cgen::visitUnaryExpr(Unary* expr) {}
void Cgen::visitVariableExpr(Variable* expr) {}
void Cgen::visitNewExpr(New* expr) {}
void Cgen::visitBlockExpr(Block* expr) {}
void Cgen::visitGroupingExpr(Grouping* expr) {}
void Cgen::visitStaticDispatchExpr(StaticDispatch* expr) {}
void Cgen::visitDispatchExpr(Dispatch* expr) {}
void Cgen::visitLiteralExpr(Literal* expr) {}
void Cgen::visitLetExpr(Let* expr) {}
void Cgen::visitCaseExpr(Case* expr) {}



}
