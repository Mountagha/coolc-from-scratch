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

}
