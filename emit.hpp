#pragma once
///////////////////////////////////////////////////////////////////////
//
//  Assembly Code Naming Conventions:
//
//     Dispatch table            <classname>_dispTab
//     Method entry point        <classname>.<method>
//     Class init code           <classname>_init
//     Abort method entry        <classname>.<method>.Abort
//     Prototype object          <classname>_protObj
//     Integer constant          int_const<Symbol>
//     String constant           str_const<Symbol>
//
///////////////////////////////////////////////////////////////////////

// Garbage collector management.
#define cgen_Memmgr 0
#define cgen_Memmgr_Test 0
#define GC_TEST 1
#define CGEN_DEBUG 1

#define MAXINT  100000000    
#define WORD_SIZE    4
#define LOG_WORD_SIZE 2     // for logical shifts
#define FILENAME "str_const0"

// Global names
#define CLASSNAMETAB         "class_nameTab"
#define CLASSOBJTAB          "class_objTab"
#define INTTAG               "_int_tag"
#define BOOLTAG              "_bool_tag"
#define STRINGTAG            "_string_tag"
#define HEAP_START           "heap_start"

// Naming conventions
#define DISPTAB_SUFFIX       "_dispTab"
#define UNDERSCORE           "_"
#define METHOD_SEP           "."
#define CLASSINIT_SUFFIX     "_init"
#define PROTOBJ_SUFFIX       "_protObj"
#define OBJECTPROTOBJ        "Object" PROTOBJ_SUFFIX
#define INTCONST_PREFIX      "int_const"
#define STRCONST_PREFIX      "str_const"
#define BOOLCONST_PREFIX     "bool_const"
#define BOOLCONST_TRUE       "bool_const1"
#define BOOLCONST_FALSE      "bool_const0"


#define EMPTYSLOT            0
#define LABEL                ":\n"

#define STRINGNAME (char *) "String"
#define INTNAME    (char *) "Int"
#define BOOLNAME   (char *) "Bool"
#define MAINNAME   (char *) "Main"

//
// information about object headers
//
#define DEFAULT_OBJFIELDS 3
#define AR_BASE_SIZE 3
#define TAG_OFFSET 0
#define SIZE_OFFSET 1
#define DISPTABLE_OFFSET 2

#define STRING_SLOTS      1
#define INT_SLOTS         1
#define BOOL_SLOTS        1

#define GLOBAL        "\t.globl\t"
#define ALIGN         "\t.align\t2\n"
#define WORD          "\t.word\t"
#define ASCII         "\t.ascii\t"
#define ASCIIZ        "\t.asciiz\t"
#define SPACE        "\t.space\t"
#define BYTE        "\t.byte\t"

//
// register names
//
#define ZERO "zero"		// Zero register 
#define ACC  "a0"		// Accumulator 
#define A1   "a1"		// For arguments to prim funcs 
#define SELF "s0"		// Ptr to self (callee saves) 
#define S1   "s1"
#define S2   "s2"
#define T1   "t1"		// Temporary 1 
#define T2   "t2"		// Temporary 2 
#define T3   "t3"		// Temporary 3 
#define T4   "t4"		// Temporary 4
#define T5   "t5"		// Temporary 5
#define SP   "sp"		// Stack pointer 
#define FP   "fp"		// Frame pointer 
#define RA   "ra"		// Return address 

//
// Opcodes
//
#define JUMP  "\tj\t"  
#define JR    "\tjr\t$"  
#define JALR  "\tjalr\t$"  
#define JAL   "\tjal\t"                 
#define RET   "\tjr\t" RA "\t"

#define SW    "\tsw\t$"
#define SB    "\tsb\t$"
#define LA    "\tla\t$"
#define LB    "\tlb\t$"
#define LD    "\tld\t$"
#define LW    "\tlw\t$"
#define LI    "\tli\t$"
#define LUI    "\tlui\t$"

#define MOVE  "\tmove\t$"
#define NEG   "\tneg\t$"
#define NOR   "\tnor\t$"
#define OR   "\tor\t$"
#define XOR   "\txor\t$"
#define AND   "\tand\t$"
#define ADD   "\tadd\t$"
#define ADDI  "\taddi\t$"
#define ADDU  "\taddu\t$"
#define ADDIU "\taddiu\t$"
#define DIV   "\tdiv\t$"
#define DIVU   "\tdivu\t$"
#define MUL   "\tmul\t$"
#define SUB   "\tsub\t$"
#define SLL   "\tsll\t$"
#define B     "\tb\t"
#define BEQZ  "\tbeqz\t$"
#define BRANCH   "\tb\t"
#define BEQ      "\tbeq\t$"
#define BNE      "\tbne\t$"
#define BGE      "\tbge\t$"
#define BLEQ     "\tble\t$"
#define BLT      "\tblt\t$"
#define BGT      "\tbgt\t$"
#define SEQ      "\tseq\t$"
#define SGE      "\tsge\t$"
#define SGT      "\tsgt\t$"
#define SLE      "\tsle\t$"
#define SNE      "\tsne\t$"

