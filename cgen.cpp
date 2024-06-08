#include <stack>

#include "cgen.hpp"
#include "emit.hpp"
#include <algorithm>


namespace cool {

// Used by the Garbage collector.
static std::vector<std::string> gc_init_names = 
{ "_NoGC_Init", "_GenGC_Init", "_ScnGC_Init" };
static std::vector<std::string> gc_collect_names = 
{ "_NoGC_Collect", "_GenGC_Collect", "_ScnGC_Collect" };



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

void Cgen::emit_not(const char* dest, const char* src) { // dest = ~src
    os << NOR << dest << ", $" << src << ", $" << src << std::endl;
}

void Cgen::emit_not(const char* reg) { // src = ~src
    os << NOR << reg << ", $" << reg << ", $" << reg << std::endl;
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

void Cgen::emit_bgt(const char* src1, int imm, const std::string& label) {
    os << BGT << src1 << ", " << imm << ", " << label << std::endl;
}

void Cgen::emit_blt(const char* src1, int imm, const std::string& label) {
    os << BLT << src1 << ", " << imm << ", " << label << std::endl;
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

void Cgen::emit_sw(const char* src, int offset, const char* dest) {
    os << SW << src << ", " << offset << "($" << dest << ")" << std::endl;
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

void Cgen::emit_pop(const char* reg) {
    emit_lw(reg, WORD_SIZE, SP);
    emit_pop(1);
}

void Cgen::emit_push(const char* reg) {
    emit_addiu(SP, SP, -WORD_SIZE);
    emit_sw(reg, WORD_SIZE, SP);
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

void Cgen::emit_init_ref(const char* s) {
    os << s << CLASSINIT_SUFFIX;
}

void Cgen::emit_label(const std::string& label) {
    os << label << ":\n";
}

void Cgen::emit_comment(const std::string& s) {
    os << "# " << s << std::endl;
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

void Cgen::print_string_in_bytes(const std::string& s) {
    std::size_t column_counter = 0, column_limit = 15;
    os << BYTE;
    for (auto& c: s) {
        os << static_cast<int>(c) << " ";
        column_counter++;
        if (column_counter > column_limit) {
            os << std::endl << BYTE;
            column_counter = 0;
        }
    }
    os << std::endl;
}

void Cgen::print_string_literal (const std::string& s) {

    bool is_new_ascii = true;
    for (int i=0; i < s.size(); i++) {
        if (s[i] == '\\' && s[i+1] == '\\') {
            if (!is_new_ascii) {
                this->os << "\"" << std::endl;
            }
            this->os << BYTE << static_cast<int>('\\') << std::endl; // print ascii code.
            is_new_ascii = true;
            i++;
        } else {
            if (is_new_ascii) {
                this->os << ASCII << "\"" << s[i];
                is_new_ascii = false;
            } else {
                this->os << s[i];  // Print the character as is
            }
        }
    }
    if (!is_new_ascii)
        this->os << "\"" << std::endl;
}

void Cgen::code_constants() {
    //
    // Add constants that are required by the code generator
    //
    // add class names to string constants and their corresponding lengths to inttable. 
    for (auto& class_: g->get_graph()) {
        stringtable().insert(class_.first.lexeme, class_.first);
        inttable().insert(std::to_string(class_.first.lexeme.size()), class_.first);
    }
    
    // add empty string to string const table since it's the default value 
    // value of a newly allocated string.
    // add 0 to the int entry [in case it isn't present] for the same reason.
    stringtable().insert("", Token{TokenType::STRING, ""});
    inttable().insert("0", Token{TokenType::NUMBER, ""});
    auto contains_unrecognized_char = [](const std::string s) -> bool {
        // spim somehow do not recognized `\\` so we print it ascii code instead of literal value.
        char previous;
        for (auto& c: s) {
            if (c == '\\' && previous == '\\')
                return true;
            previous = c;
        }
        return false;
    };

    for (auto& elt: stringtable().get_elements()) {
        
        int idx = stringtable().get_index(elt.first);
        int string_obj_size = elt.first.size() % 4 == 0 ? elt.first.size() / 4 : elt.first.size() / 4 + 1;
        os << STRCONST_PREFIX << idx << LABEL;                                                // label
        os << WORD << STRING_CLASS_TAG << std::endl;                                            // tag 
        os << WORD << (DEFAULT_OBJFIELDS + STRING_SLOTS + string_obj_size) << std::endl;   // size
        os << WORD << "String" << DISPTAB_SUFFIX << std::endl;
        os << WORD << INTCONST_PREFIX << inttable().get_index(std::to_string(elt.first.size())) << std::endl;
        if (contains_unrecognized_char(elt.first))
            print_string_literal(elt.first);
        else
            os << ASCII << "\"" << elt.first.c_str() << "\"\n";
        os << BYTE << 0 << std::endl;
        os << ALIGN;
        os << WORD << -1 << std::endl;

    }

    for (auto& elt: inttable().get_elements()) {

        int idx = inttable().get_index(elt.first);
        os << INTCONST_PREFIX << idx << LABEL;                                                // label
        os << WORD << INT_CLASS_TAG << std::endl;
        os << WORD << (DEFAULT_OBJFIELDS + INT_SLOTS) << std::endl;
        os << WORD << "Int" << DISPTAB_SUFFIX << std::endl;
        os << WORD << elt.first.c_str() << std::endl;
        os << WORD << -1 << std::endl;

    }

    // code gen for bools
    os << BOOLCONST_FALSE << LABEL; // false
    os << WORD << BOOL_CLASS_TAG << std::endl;
    os << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << std::endl; 
    os << WORD << "Bool" << DISPTAB_SUFFIX << std::endl;
    os << WORD << "0" << std::endl;

    os << BOOLCONST_TRUE << LABEL; // true
    os << WORD << BOOL_CLASS_TAG << std::endl;
    os << WORD << (DEFAULT_OBJFIELDS + BOOL_SLOTS) << std::endl; 
    os << WORD << "Bool" << DISPTAB_SUFFIX << std::endl;
    os << WORD << "1" << std::endl;

}

void Cgen::construct_classtag_map() {

    int classtag = 8;    // to avoid clash with basic class values
    classtag_map.insert({Object.lexeme, OBJECT_CLASS_TAG});
    for (auto& class_: g->DFS(Object)) {

        if (classtag_map.find(class_.lexeme) != classtag_map.end()) 
            continue;
        if (class_ == Str)
            classtag_map.insert({class_.lexeme, STRING_CLASS_TAG});
        else if (class_ == Int)
            classtag_map.insert({class_.lexeme, INT_CLASS_TAG});
        else if (class_ == Bool)
            classtag_map.insert({class_.lexeme, BOOL_CLASS_TAG});
        else {
            classtag_map.insert({class_.lexeme, classtag});
            classtag++;
        }
    }
    for (auto& kv: classtag_map) {
        std::cout << kv.first << " " << kv.second << std::endl;
    }
}

void Cgen::class_name_table() {
    
    // class_name table
    // we want to retrieve the names of the different classes
    // in an ascending order of their respectives tags.
    std::vector<std::pair<std::string, int>> class_tag_pairs (classtag_map.begin(), classtag_map.end());
    std::sort(class_tag_pairs.begin(), class_tag_pairs.end(), []
        (const std::pair<std::string, int>& a, const std::pair<std::string, int>& b){
            return a.second < b.second;
        }
    );
    os << CLASSNAMETAB << LABEL;
    os << SPACE << 4 * 4 << std::endl; // since the first class (Object) Index start at 4 add a padding of 16 bytes 
    for (auto& v: class_tag_pairs) {
        int idx = stringtable().get_index(v.first); // we sure to get an index since classes are added previously
        os << WORD << STRCONST_PREFIX << idx << std::endl;
    }
}

void Cgen::code_dispatch_table(Class* class_) {
    std::map<Token, Token>  mnames; // maps of all methods names
    // the methods in the dispatch table must be ordered
    // starting from the top of the hierarchy down to the class
    // class_node is pointing to
    std::stack<Class*> class_ordering;

    Class* curr_class = class_;

    // go up the inheritance tree and for each class, push it to the stack
    // (so class Object will be on the top of the stack after this loop) and
    // all the method names in mnames.
    // this is used to check later if a method was overriden. 
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

    // go through the stack (goes down the inheritance tree starting from Object)

    while (!class_ordering.empty()) {
        curr_class = class_ordering.top();

        // for each method in the current class, if the method is still in the mnames
        // table (not overriden), add it to the method table with the offset then
        // remove the method so there won't be any duplication if a method
        // is overriden by a derived class

        for (auto& m: curr_class->features) {
            if (mnames.find(m->id) != mnames.end()) {
                method_table[class_->name.lexeme][m->id.lexeme] = dispoffset++;
                os << WORD << mnames[m->id].lexeme << METHOD_SEP << m->id.lexeme << std::endl;
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

    int current_attribut_offset = 0;

    while (!classes.empty()) {
        curr_class = classes.top();

        for (auto& f: curr_class->features)
            if (f->featuretype == FeatureType::ATTRIBUT) {
                os << WORD << "0" << std::endl;
                attr_table[class_->name.lexeme][f->id.lexeme] = ++current_attribut_offset;
            }
        classes.pop();
    }
}


void Cgen::code_prototype_objects() {
    int classtag = 8;    // to avoid clash with basic class values

    for (auto& class_: classtag_map) {

        os << class_.first << PROTOBJ_SUFFIX << LABEL;
        os << WORD << class_.second << std::endl;
        os << WORD << (DEFAULT_OBJFIELDS + calc_obj_size(class_table_ptr->get(class_.first))) << std::endl;
        os << WORD << class_.first << DISPTAB_SUFFIX << std::endl;
        emit_obj_attributes(class_table_ptr->get(class_.first));
    }
}

void Cgen::code_global_data() {

    os << ".data\n" << ALIGN;

    // The following global names should be defined first.
    os << GLOBAL << CLASSNAMETAB << std::endl;
    os << GLOBAL; emit_protobj_ref(STRINGNAME); os << std::endl; 
    os << GLOBAL; emit_protobj_ref(INTNAME); os << std::endl; 
    os << GLOBAL; emit_protobj_ref(MAINNAME); os << std::endl; 
    os << GLOBAL; os << BOOLCONST_FALSE << std::endl; 
    os << GLOBAL; os << BOOLCONST_FALSE << std::endl; 
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
       << WORD << STRING_CLASS_TAG << std::endl;

    if (CGEN_DEBUG) code_select_gc();
}

void Cgen::code_global_text() {
    os << GLOBAL << HEAP_START << std::endl;
    os << HEAP_START << LABEL << WORD << 0 << std::endl;
    os << "\t.text" << std::endl;

    os << GLOBAL; emit_init_ref(MAINNAME); os << std::endl;
    os << GLOBAL; emit_init_ref(INTNAME); os << std::endl;
    os << GLOBAL; emit_init_ref(STRINGNAME); os << std::endl;
    os << GLOBAL; emit_init_ref(BOOLNAME); os << std::endl;
    os << GLOBAL << MAINNAME << METHOD_SEP << "main" << std::endl;
}

void Cgen::code_select_gc() {
    
    // Generate GC choice constants (pointers to GC functions)
    os << GLOBAL << "_MemMgr_INITIALIZER" << std::endl;
    os << "_MemMgr_INITIALIZER:" << std::endl;
    os << WORD <<  gc_init_names[cgen_Memmgr]  << std::endl;
    os << GLOBAL << "_MemMgr_COLLECTOR" << std::endl;
    os << "_MemMgr_COLLECTOR:" << std::endl;
    os << WORD <<  gc_collect_names[cgen_Memmgr]  << std::endl;
    os << GLOBAL << "_MemMgr_TEST" << std::endl;
    os << "_MemMgr_TEST:" << std::endl;
    os << WORD <<  (cgen_Memmgr_Test == GC_TEST)  << std::endl;
}

void Cgen::cgen_init_formal(Token& formal_type) {
    // switch case could be nicer but hey restrictions on switch case with tokenType
    if (formal_type == Int) 
        emit_la(ACC, std::string(INTCONST_PREFIX) + std::to_string(inttable().get_index("0")));
    else if (formal_type == Str)
        emit_la(ACC, std::string(STRCONST_PREFIX) + std::to_string(stringtable().get_index("")));
    else if (formal_type == Bool)
        emit_la(ACC, BOOLCONST_FALSE);
    else  
        emit_move(ACC, ZERO);
}

// Cgen for Exprs and Stmts
void Cgen::visitProgramStmt(Program* stmt) {
    std::cout << "debut code generation\n\n";

    code_global_data();

    construct_classtag_map();

    code_constants();

    class_name_table();

    localsizer.computeSize(stmt);

    std::cout << "Real code actually starting here.\n\n";
    for(auto& p: g->get_graph()) {
        auto class_ = class_table_ptr->get(p.first.lexeme);
        os << class_->name.lexeme << DISPTAB_SUFFIX << LABEL;
        code_dispatch_table(class_);
    }

    code_prototype_objects();

    code_global_text(); 

    // Codegen the basic classes first.
    std::vector<std::string> basic_classes = 
            {"Object", "IO", "String", "Int", "Bool"};
    for (auto& cname: basic_classes) {
        curr_class = class_table_ptr->get(cname);
        class_table_ptr->get(cname)->accept(this);
    }

    // Then codegen class declared by user.
    for (auto& class_: stmt->classes) {
        curr_class = class_.get();
        class_->accept(this);
    }

    std::cout << "fin code generation\n\n";
}


void Cgen::visitClassStmt(Class* stmt) {

    // as each class node is traversed, its _init method (akin to constructor)
    // is also generated.

    var_env.enterScope();
    Token classname = stmt->name;
    os << classname.lexeme + CLASSINIT_SUFFIX << LABEL;

    // reserve space for AR (old frame pointer + self object + return adress + potential local variables[cases, let])
    size_t object_size = AR_BASE_SIZE;
    if (!is_base_class(curr_class))
        object_size += localsizer.getClassLocalSize(curr_class->name.lexeme);
    emit_push(object_size);

    // standard registers that are saved to the stack
    emit_sw(FP, object_size * WORD_SIZE, SP);
    emit_sw(SELF, object_size * WORD_SIZE - WORD_SIZE, SP);
    emit_sw(RA, 4, SP);
    emit_addiu(FP, SP, 4);
    emit_move(SELF, ACC);

    class_fp_offset = 1;

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
    emit_lw(FP, object_size * WORD_SIZE, SP);
    emit_lw(SELF, object_size * WORD_SIZE - WORD_SIZE, SP);
    emit_lw(RA, 4, SP);
    emit_pop(object_size);
    emit_jr(RA);

    curr_attr_count = 0;
    for (auto& method: stmt->features) {
        if (method->featuretype == FeatureType::METHOD)
            method->accept(this);
    }

    var_env.exitScope();
}

void Cgen::cgen_attribut(Feature* attr) {
    if (attr->expr)
        attr->expr->accept(this);
    else // init the attr with the default value type
        if (attr->type_ != prim_slot)
            cgen_init_formal(attr->type_);

    // PRIM_SLOT refers to an attribute of a primitive type (eg. Bool, String, Int)
    // the current attribute counter is incremented by 2 since the starting offset
    // for an attribute in the object layout if offset 3 (offset 0-2 being the headers)
    // and then multiplied by 4 since there are 4 bytes in a word.
    int offset = attr_table[curr_class->name.lexeme][attr->id.lexeme];
    if (attr->type_ != prim_slot) 
        emit_sw(ACC, WORD_SIZE * (offset + 2), SELF);

}

void Cgen::cgen_method(Feature* method) {
    if (is_base_class(curr_class))
        return;

    inside_function = true; 
    std::size_t ar_size = AR_BASE_SIZE + method->formals.size() + localsizer.getFuncLocalSize(method->id.lexeme);
    var_env.enterScope();
    emit_label(curr_class->name.lexeme + METHOD_SEP + method->id.lexeme);
    if (method->id == main_meth) {
        // No dispatch prior to main hence doing allocation inside and registers save here.
        emit_push(ar_size);
        emit_sw(FP, ar_size * WORD_SIZE, SP);
        emit_sw(SELF, ar_size * WORD_SIZE - WORD_SIZE, SP);
    }
    emit_sw(RA, 4, SP);

    //int curr_offset = 1; !TODO double check later
    fp_offset = 1;
    for(auto& f: method->formals) {
        var_env.insert(f->id.lexeme, fp_offset);
        fp_offset++;
    }

    emit_move(SELF, ACC);
    method->expr->accept(this);

    // refer to stack frame layout in header file
    emit_lw(FP, ar_size * WORD_SIZE, SP);
    emit_lw(SELF, ar_size * WORD_SIZE - WORD_SIZE, SP);
    emit_lw(RA, 4, SP);
    emit_pop(ar_size);
    emit_jr(RA);

    var_env.exitScope();

    inside_function = false;

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


void Cgen::visitFormalExpr(Formal* expr) {
    // nothing to do here.
}

void Cgen::visitAssignExpr(Assign* expr) {
    expr->expr->accept(this);
    int *offset = var_env.get(expr->id.lexeme);

    // result of evaluating rhs of assignment 
    // is expected to be in the register ACC
    // also check that offset is not checked for 
    // null because the semantic analyzer should've 
    // caught any variable misuse by this point

    // !TODO not sure here cause different from 
    // (inspired repo file astnodecodegenerator.cpp:663)
    if (offset) // local var
        emit_sw(ACC, (*offset) * WORD_SIZE, FP);
    else // attribute
        emit_sw(ACC, WORD_SIZE * ( attr_table[curr_class->name.lexeme][expr->id.lexeme] + 2 ), SELF);

}

void Cgen::visitIfExpr(If* expr) {

    ifcount++;
    std::string label_suffix = std::to_string(ifcount);
    expr->cond->accept(this);

    emit_la(T1, BOOLCONST_TRUE); // bool_const1
    emit_beq(T1, ACC, "iftrue_branch" + label_suffix);
    emit_label("iffalse_branch" + label_suffix);
    expr->elseBranch->accept(this);
    emit_b("end_if" + label_suffix);   
    emit_label("iftrue_branch" + label_suffix);
    expr->thenBranch->accept(this);
    emit_label("end_if" + label_suffix);   

}

void Cgen::visitWhileExpr(While* expr) {

    while_count++;
    std::string label_suffix = std::to_string(while_count);
    emit_label("while_branch" + label_suffix);
    expr->cond->accept(this);

    emit_la(T1, BOOLCONST_TRUE); // bool_const1
    emit_bne(T1, ACC, "end_while_branch" + label_suffix);

    expr->expr->accept(this);

    emit_b("while_branch" + label_suffix);
    emit_label("end_while_branch" + label_suffix);
    emit_li(ACC, 0); // a while always returns null.

}

void Cgen::visitBinaryExpr(Binary* expr) {

    expr->lhs->accept(this);

    switch (expr->op.token_type) {
        case PLUS:

            emit_push(ACC);
            expr->rhs->accept(this);
            emit_jal("Object.copy");
            emit_lw(T1, 4, SP);
            emit_lw(T1, 12, T1);
            emit_lw(T2, 12, ACC);
            emit_add(T1, T1, T2);
            emit_sw(T1, 12, ACC);
            emit_pop(1);
            break;

        case MINUS:

            emit_push(ACC);
            expr->rhs->accept(this);
            emit_jal("Object.copy");
            emit_lw(T1, 4, SP);
            emit_lw(T1, 12, T1);
            emit_lw(T2, 12, ACC);
            emit_sub(T1, T1, T2);
            emit_sw(T1, 12, ACC);
            emit_pop(1);
            break;

        case STAR:

            emit_push(ACC);
            expr->rhs->accept(this);
            emit_jal("Object.copy");
            emit_lw(T1, 4, SP);
            emit_lw(T1, 12, T1);
            emit_lw(T2, 12, ACC);
            emit_mul(T1, T1, T2);
            emit_sw(T1, 12, ACC);
            emit_pop(1);
            break;

        case SLASH:

            emit_push(ACC);
            expr->rhs->accept(this);
            emit_jal("Object.copy");
            emit_lw(T1, 4, SP);
            emit_lw(T1, 12, T1);
            emit_lw(T2, 12, ACC);
            emit_div(T1, T1, T2);
            emit_sw(T1, 12, ACC);
            emit_pop(1);
            break;

        case LESS:

            emit_push(ACC);
            expr->rhs->accept(this);
            emit_pop(S1);
            emit_jal("less");
            break;

        case LESS_EQUAL:

            emit_push(ACC);
            expr->rhs->accept(this);
            emit_pop(S1);
            emit_jal("less_eq");
            break;

        case EQUAL:

            emit_push(ACC);
            expr->rhs->accept(this);
            emit_pop(S1);
            emit_jal("eq");
            break;
    }
}

void Cgen::visitUnaryExpr(Unary* expr) {
    expr->expr->accept(this);

    switch (expr->op.token_type) {
        case TILDE:
            // ~ is used on integer only hence the offset 12 to get 
            // the value.
            emit_jal("Object.copy");
            emit_lw(T1, 12, ACC);
            emit_not(T2, T1),
            emit_sw(T2, 12, ACC);
            break;

        case NOT:
            emit_jal("lnot");
            break;

        case ISVOID:
            emit_jal("isvoid");
            break;
    }
    
}

void Cgen::visitVariableExpr(Variable* expr) {
    if (expr->name == self) {
        emit_move(ACC, SELF);
    } else {

        // if the variable name is not in the current local scope
        // check if it's an attribute of the current class.
        int *offset = var_env.get(expr->name.lexeme);
        if (offset)
                emit_lw(ACC, (*offset) * WORD_SIZE, FP);
        else {
            emit_lw(ACC, WORD_SIZE * (attr_table[curr_class->name.lexeme][expr->name.lexeme] + 2), SELF);
        } 
    }
}

void Cgen::visitNewExpr(New* expr) {
    emit_la(ACC, expr->expr_type.lexeme + PROTOBJ_SUFFIX);
    emit_jal("Object.copy");
    emit_jal(expr->expr_type.lexeme + CLASSINIT_SUFFIX);
}

void Cgen::visitBlockExpr(Block* expr) {
    for(auto& e: expr->exprs) {
        e->accept(this);
    }
}

void Cgen::visitGroupingExpr(Grouping* expr) {
    expr->expr->accept(this);
}

void Cgen::visitStaticDispatchExpr(StaticDispatch* expr) {

    
    std::size_t ar_size = AR_BASE_SIZE + expr->args.size();
    if (!is_base_function(expr->callee_name))
        ar_size += localsizer.getFuncLocalSize(expr->callee_name.lexeme);
    
    emit_push(ar_size);
    emit_sw(FP, ar_size * WORD_SIZE, SP);
    emit_sw(SELF, ar_size * WORD_SIZE - WORD_SIZE, SP);


    std::size_t formal_offset = 8;
    for (auto& arg: expr->args) {
        arg->accept(this);
        emit_sw(ACC, formal_offset, SP);
        formal_offset += WORD_SIZE;
    }

    expr->expr->accept(this);
    emit_addiu(FP, SP, 4);

    emit_bne(ACC, ZERO, "DispatchLabel" + std::to_string(dispatch_count));
    emit_la(ACC, FILENAME);
    emit_li(T1, 1);
    emit_jal("_dispatch_abort");
    emit_label("DispatchLabel" + std::to_string(dispatch_count)); 
    dispatch_count++;
    // code for dispatch
    emit_la(T1, expr->class_.lexeme + std::string(PROTOBJ_SUFFIX));
    emit_lw(T1, 8, T1); // to get the dispatch table pointer.
    emit_lw(T1, method_table[expr->class_.lexeme][expr->callee_name.lexeme] * WORD_SIZE, T1);
    emit_jalr(T1);
}

void Cgen::visitDispatchExpr(Dispatch* expr) {
    
    std::size_t ar_size = AR_BASE_SIZE + expr->args.size();
    if (!is_base_function(expr->callee_name))
        ar_size += localsizer.getFuncLocalSize(expr->callee_name.lexeme);

    emit_push(ar_size);
    emit_sw(FP, ar_size * WORD_SIZE, SP);
    emit_sw(SELF, ar_size * WORD_SIZE - WORD_SIZE, SP);


    std::size_t formal_offset = 8;
    for (auto& arg: expr->args) {
        arg->accept(this);
        emit_sw(ACC, formal_offset, SP);
        formal_offset += WORD_SIZE;
    }

    expr->expr->accept(this);
    emit_addiu(FP, SP, 4);

    // dispatch error on void
    emit_bne(ACC, ZERO, "DispatchLabel" + std::to_string(dispatch_count));
    emit_la(ACC, FILENAME);
    emit_li(T1, 1);
    emit_jal("_dispatch_abort");
    // code for dispatch
    emit_label("DispatchLabel" + std::to_string(dispatch_count));
    dispatch_count++;
    emit_lw(T1, 8, ACC); // to get the dispatch table pointer.
    emit_lw(T1, method_table[expr->expr->expr_type.lexeme][expr->callee_name.lexeme] * WORD_SIZE, T1);
    emit_jalr(T1);
}

void Cgen::visitLiteralExpr(Literal* expr) {

    switch (expr->object.type()) {
        case CoolType::Bool_t:
            if (expr->object.bool_value())
                emit_la(ACC, BOOLCONST_TRUE);
            else 
                emit_la(ACC, BOOLCONST_FALSE);
            break;
        case CoolType::Number_t:
            emit_la(ACC, std::string(INTCONST_PREFIX) + std::to_string(inttable().get_index(std::to_string(expr->object.int_value()))));
            break;
        case CoolType::String_t:
            emit_la(ACC, std::string(STRCONST_PREFIX) + std::to_string(stringtable().get_index(expr->object.string_value())));
            break;
        case CoolType::Void_t:
            emit_move(ACC, ZERO);
            break;

    }
}

void Cgen::visitLetExpr(Let* expr) {

    var_env.enterScope();
    for (auto& let: expr->vecAssigns) {
        // codegen all the expressions in the let init if exists.
        Expr* let_expr = std::get<1>(let).get();
        Token let_id = std::get<0>(let).get()->id;
        Token let_type = std::get<0>(let).get()->type_; 
        if (let_expr) {
            let_expr->accept(this);
        } else { // use default initialization.
            cgen_init_formal(let_type);
        }
        if (inside_function) {
            emit_sw(ACC, fp_offset * WORD_SIZE, FP);
            var_env.insert(let_id.lexeme, fp_offset);
            fp_offset++;
        } else { // a let that initialize an attribute
            emit_sw(ACC, class_fp_offset * WORD_SIZE, FP);
            var_env.insert(let_id.lexeme, class_fp_offset);
            class_fp_offset++;
        }
    }
    expr->body->accept(this);
    emit_comment("Let ends here");
    var_env.exitScope();
}

void Cgen::visitCaseExpr(Case* expr) {

    // a lambda to find the child class with the highest tag of a certain
    // class hierarchy.
    auto max_inherited_class_tag = [this](Token& class_name) -> int {
        int max_tag = this->classtag_map[class_name.lexeme]; // the lowest.
        for(auto& class_: this->g->DFS(class_name)) {
            if (this->classtag_map[class_.lexeme] > max_tag)
                    max_tag = this->classtag_map[class_.lexeme];
        }
        return max_tag;
    };
    emit_comment("Label construct starts here.");
    expr->expr->accept(this);
    int tagCaseEnd = casecount++;
    emit_bne(ACC, ZERO, "CaseLabel" + std::to_string(casecount));
    emit_la(ACC, FILENAME);
    emit_li(T1, 1);
    emit_jal("_case_abort2");

    // Object case branch is to be handled last if present.
    bool there_is_object = false, first_iter=true;
    Formal* object_formal;
    Expr* obj_expr;

    // we want the lowest classes (highest tags) in the hierarchy to get 
    // the caseLabels first.
    std::sort(expr->matches.begin(), expr->matches.end(), 
    [this](letAssign& a, letAssign& b){
        return this->classtag_map[std::get<0>(a).get()->type_.lexeme] >
        this->classtag_map[std::get<0>(b).get()->type_.lexeme];
    });

    for (auto& match: expr->matches) {

        // codegen every match expression.
        auto formal = std::get<0>(match).get();
        Expr* match_expr = std::get<1>(match).get();
        if (formal->type_ == Object) {
            there_is_object = true;
            object_formal = formal;
            obj_expr = match_expr;
            continue; // handle Object branch later.
        }
        emit_label("CaseLabel" + std::to_string(casecount++));
        if (first_iter) {
            emit_lw(T2, TAG_OFFSET, ACC);
            first_iter = false;
        }
        emit_blt(T2, classtag_map[formal->type_.lexeme], "CaseLabel" + std::to_string(casecount));
        emit_bgt(T2, max_inherited_class_tag(formal->type_), "CaseLabel" + std::to_string(casecount));
        // bind idk to expr0 before evaluating exprk.
        if (inside_function) {
            emit_sw(ACC, fp_offset * WORD_SIZE, FP);
            var_env.insert(formal->id.lexeme, fp_offset);
        } else {
            emit_sw(ACC, class_fp_offset * WORD_SIZE, FP);
            var_env.insert(formal->id.lexeme, class_fp_offset);
        }
        match_expr->accept(this); 
        emit_b("CaseLabel" + std::to_string(tagCaseEnd));
    }

    if (there_is_object) {
        emit_label("CaseLabel" + std::to_string(casecount++));
        emit_blt(T2, classtag_map[object_formal->type_.lexeme], "CaseLabel" + std::to_string(casecount));
        emit_bgt(T2, max_inherited_class_tag(object_formal->type_), "CaseLabel" + std::to_string(casecount));
        obj_expr->accept(this); 
        emit_b("CaseLabel" + std::to_string(tagCaseEnd));
    }
    
    // Not found corresponding case.
    emit_label("CaseLabel" + std::to_string(casecount++));
    emit_jal("_case_abort");
    // code after the switch case.
    emit_label("CaseLabel" + std::to_string(tagCaseEnd));
}

}   // end of namespace.