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


void Cgen::emit_push(int num_words) {
    emit_addiu(SP, SP, WORD_SIZE * -num_words);
}

void Cgen::emit_pop(int num_words) {
    emit_addiu(SP, SP, WORD_SIZE * num_words);
}


void Cgen::emit_push(const char* reg) {
    emit_sw(reg, 0, SP);
    emit_addiu(SP, SP, -4);
}

void Cgen::emit_ascii(const std::string& s) {
    os << "\t.ascii\t\"" << s << "\"\n";
}

void Cgen::emit_asciiz(const std::string& s) {
    os << "\t.asciiz\t\"" << s << "\"\n";
}

void Cgen::emit_protobj_ref(const char* s) {
    os << s << PROTOBJ_SUFFIX;
}

void Cgen::emit_label(const std::string& label) {
    os << label << ":\n";
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
    for (auto& class_: g->get_graph()) {
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
    
    while (true) {
        class_ordering.push(curr_class);
        for(auto& m: curr_class->features) {
            if (m->featuretype == FeatureType::METHOD) 
                if (mnames.find(m->id) == mnames.end())
                    mnames.insert({m->id, curr_class->name});
        }
        Token parent = curr_class->superClass;
        if (parent == No_class)
            break;
        curr_class = class_table_ptr->get(parent.lexeme); 
    }

    int dispoffset = 0; 

    while (!class_ordering.empty()) {
        curr_class = class_ordering.top();

        for (auto& m: curr_class->features) {
            if (mnames.find(m->id) != mnames.end()) {
                method_table[curr_class->name.lexeme][m->id.lexeme] = dispoffset++;
                os << WORD << curr_class->name.lexeme << METHOD_SEP << m->id.lexeme << std::endl;
                mnames.erase(m->id);
            }
        }
        class_ordering.pop();
    }
}

int Cgen::calc_obj_size(Class* class_) {
    int total = 0;
    Class* curr_class = class_;

    while (true) {
        for(auto& f: curr_class->features) {
            if (f->featuretype == FeatureType::ATTRIBUT) {
                total++;
            }
        }
        Token parent = curr_class->superClass; 
        if (parent == No_class)
            break;
        curr_class = class_table_ptr->get(parent.lexeme);
    }
    
    return total;
}

void Cgen::emit_obj_attributes(Class* class_) {

    std::stack<Class*> classes;
    Class* curr_class = class_;
    while(true) {
        classes.push(curr_class);
        Token parent = curr_class->superClass;
        if (parent == No_class) 
            break;
        curr_class = class_table_ptr->get(parent.lexeme);
    }

    while (!classes.empty()) {
        curr_class = classes.top();

        for (auto& f: curr_class->features)
            if (f->featuretype == FeatureType::ATTRIBUT)
                os << WORD << "0" << std::endl;
        classes.pop();
    }
}


void Cgen::code_prototype_objects() {
    int classtag = 8;    // to avoid clash with basic class values
    for (auto& class_: g->get_graph()) {

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

        os << WORD << (DEFAULT_OBJFIELDS + calc_obj_size(class_table_ptr->get(class_.first.lexeme)));
        os << WORD << class_.first.lexeme << DISPTAB_SUFFIX << std::endl;
        emit_obj_attributes(class_table_ptr->get(class_.first.lexeme));
    }
}

void Cgen::code_global_data() {

    os << ".data\n" << ALIGN;

    // The following global names should be defined first.
    
    os << GLOBAL; emit_protobj_ref("Main"); os << std::endl; 
    os << GLOBAL; emit_protobj_ref("Int"); os << std::endl; 
    os << GLOBAL; emit_protobj_ref("String"); os << std::endl; 
    os << GLOBAL; os << "bool_const0" << std::endl; 
    os << GLOBAL; os << "bool_const1" << std::endl; 
    os << GLOBAL << INTTAG << std::endl;
    os << GLOBAL << BOOLTAG << std::endl;
    os << GLOBAL << STRINGTAG << std::endl;

    // We also need to know the tag of the Int, String and Bool classes
    // during code generation.

    os << INTTAG << LABEL
       << WORD << INT_CLASS_TAG << std::endl;
    os << BOOLTAG << LABEL
       << WORD << BOOL_CLASS_TAG << std::endl;
    os << STRINGTAG << LABEL
       << WORD << BOOL_CLASS_TAG << std::endl;
}

// Cgen for Exprs and Stmts
void Cgen::visitProgramStmt(Program* stmt) {
    std::cout << "debut code generation\n\n";

    code_global_data();
    code_constants();

    std::cout << "Real code actually starting here.\n\n";
    for(auto& p: g->get_graph()) {
        auto class_ = class_table_ptr->get(p.first.lexeme);
        os << class_->name.lexeme << DISPTAB_SUFFIX << LABEL;
        code_dispatch_table(class_);
    }

    code_prototype_objects();

    os << ".text\n";

    for (auto& class_: stmt->classes) {
        curr_class = class_.get();
        class_->accept(this);
    }

    std::cout << "fin code generation\n\n";
}


void Cgen::visitClassStmt(Class* stmt) {

    Token classname = stmt->name;
    os << classname.lexeme + CLASSINIT_SUFFIX << LABEL;

    // reserve space for AR (old frame pointer + self object + return adress)
    emit_push(AR_BASE_SIZE);

    // standard registers that are saved to the stack
    emit_sw(FP, 12, SP);
    emit_sw(SELF, 8, SP);
    emit_sw(RA, 4, SP);
    emit_addiu(FP, SP, 4);
    emit_move(SELF, ACC);


    // if the class is anything other than the object class, call
    // base class init method
    if (classname != Object)
        emit_jal(stmt->superClass.lexeme + CLASSINIT_SUFFIX);

    // emit code for attributes
    for (auto& attrib: stmt->features) {
        if (attrib->featuretype == FeatureType::ATTRIBUT)
            attrib->accept(this);
    }

    // restore registers
    emit_move(ACC, SELF);
    emit_lw(FP, 12, SP);
    emit_lw(SELF, 8, SP);
    emit_lw(RA, 4, SP);
    emit_pop(AR_BASE_SIZE);
    emit_jr(RA);

    curr_attr_count = 0;
    for (auto& method: stmt->features) {
        if (method->featuretype == FeatureType::METHOD)
            method->accept(this);
    }
}

void Cgen::cgen_attribut(Feature* attr) {
    attr->expr->accept(this);

    ++curr_attr_count;
    attr_table[curr_class->name.lexeme][attr->id.lexeme] = curr_attr_count;

    // PRIM_SLOT refers to an attribute of a primitive type (eg. Bool, String, Int)
    // the current attribute counter is incremented by 2 since the starting offset
    // for an attribute in the object layout if offset 3 (offset 0-2 being the headesr)
    // and then multiplied by 4 since there are 4 bytes in a word.
    if (attr->type_ != prim_slot) 
        emit_sw(ACC, WORD_SIZE * (curr_attr_count + 2), SELF);

}

void Cgen::cgen_method(Feature* method) {
    if (is_base_class(curr_class))
        return;
    var_env.enterScope();
    emit_label(curr_class->name.lexeme + METHOD_SEP + method->id.lexeme);

    emit_sw(RA, 4, SP);

    int curr_offset = 1;
    for(auto& f: method->formals) {
        var_env.insert(f->id.lexeme, curr_offset++);
    }

    method->expr->accept(this);

    // refer to stack frame layout in header file
    std::size_t ar_size = AR_BASE_SIZE + method->formals.size();
    emit_lw(FP, ar_size * WORD_SIZE, SP);
    emit_lw(SELF, ar_size * WORD_SIZE - WORD_SIZE, SP);
    emit_lw(RA, 4, SP);
    emit_pop(AR_BASE_SIZE + method->formals.size());
    emit_jr(RA);

    var_env.exitScope();

}

void Cgen::visitFeatureExpr(Feature* expr) {
    switch (expr->featuretype) {
        case FeatureType::ATTRIBUT:
            cgen_attribut(expr);
            break;
        case FeatureType::METHOD:
            cgen_method(expr);
            break;
    }
}


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
