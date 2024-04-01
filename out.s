.data
	.align	2
	.globl	class_nameTab
	.globl	String_protObj
	.globl	Int_protObj
	.globl	Main_protObj
	.globl	bool_const0
	.globl	bool_const0
	.globl	_int_tag
	.globl	_bool_tag
	.globl	_string_tag
_int_tag:
	.word	6
_bool_tag:
	.word	5
_string_tag:
	.word	7
	.globl	_MemMgr_INITIALIZER
_MemMgr_INITIALIZER:
	.word	_NoGC_Init
	.globl	_MemMgr_COLLECTOR
_MemMgr_COLLECTOR:
	.word	_NoGC_Collect
	.globl	_MemMgr_TEST
_MemMgr_TEST:
	.word	0
str_const_:
	.word	7
	.word	4
	.word	String_dispTab
	.word	0
	.ascii	""
	.align	2
str_const_String:
	.word	7
	.word	5
	.word	String_dispTab
	.word	6
	.ascii	"String"
	.align	2
str_const_Main:
	.word	7
	.word	5
	.word	String_dispTab
	.word	4
	.ascii	"Main"
	.align	2
str_const_Object:
	.word	7
	.word	5
	.word	String_dispTab
	.word	6
	.ascii	"Object"
	.align	2
str_const_Int:
	.word	7
	.word	4
	.word	String_dispTab
	.word	3
	.ascii	"Int"
	.align	2
str_const_IO:
	.word	7
	.word	4
	.word	String_dispTab
	.word	2
	.ascii	"IO"
	.align	2
str_const_Bool:
	.word	7
	.word	5
	.word	String_dispTab
	.word	4
	.ascii	"Bool"
	.align	2
int_const_1:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	1
bool_const0:
	.word	5
	.word	4
	.word	Bool_dispTab
	.word	0
bool_const1:
	.word	5
	.word	4
	.word	Bool_dispTab
	.word	1
Bool_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
IO_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	IO.out_string
	.word	IO.out_int
	.word	IO.in_string
	.word	IO.in_int
Int_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
Main_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	Main.main
Object_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
String_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	String.str_field
	.word	String.length
	.word	String.concat
	.word	String.substr
Bool_protObj:
	.word	5
	.word	4
	.word	Bool_dispTab
	.word	0
IO_protObj:
	.word	8
	.word	3
	.word	IO_dispTab
Int_protObj:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	0
Main_protObj:
	.word	9
	.word	3
	.word	Main_dispTab
Object_protObj:
	.word	10
	.word	3
	.word	Object_dispTab
String_protObj:
	.word	7
	.word	4
	.word	String_dispTab
	.word	0
	.globl	heap_start
heap_start:
	.word	0
	.text
	.globl	Main_init
	.globl	Int_init
	.globl	String_init
	.globl	Bool_init
	.globl	Main.main
Object_init:
	addiu	$sp, $sp, -12
	sw	$fp, 12($sp)
	sw	$s0, 8($sp)
	sw	$ra, 4($sp)
	addiu	$fp, $sp, 4
	move	$s0, $a0
	move	$a0, $s0
	lw	$fp, 12($sp)
	lw	$s0, 8($sp)
	lw	$ra, 4($sp)
	addiu	$sp, $sp, 12
	jr	$ra
IO_init:
	addiu	$sp, $sp, -12
	sw	$fp, 12($sp)
	sw	$s0, 8($sp)
	sw	$ra, 4($sp)
	addiu	$fp, $sp, 4
	move	$s0, $a0
	jal	Object_init
	move	$a0, $s0
	lw	$fp, 12($sp)
	lw	$s0, 8($sp)
	lw	$ra, 4($sp)
	addiu	$sp, $sp, 12
	jr	$ra
String_init:
	addiu	$sp, $sp, -12
	sw	$fp, 12($sp)
	sw	$s0, 8($sp)
	sw	$ra, 4($sp)
	addiu	$fp, $sp, 4
	move	$s0, $a0
	jal	Object_init
	sw	$a0, 12($s0)
	move	$a0, $s0
	lw	$fp, 12($sp)
	lw	$s0, 8($sp)
	lw	$ra, 4($sp)
	addiu	$sp, $sp, 12
	jr	$ra
Int_init:
	addiu	$sp, $sp, -12
	sw	$fp, 12($sp)
	sw	$s0, 8($sp)
	sw	$ra, 4($sp)
	addiu	$fp, $sp, 4
	move	$s0, $a0
	jal	Object_init
	move	$a0, $s0
	lw	$fp, 12($sp)
	lw	$s0, 8($sp)
	lw	$ra, 4($sp)
	addiu	$sp, $sp, 12
	jr	$ra
Bool_init:
	addiu	$sp, $sp, -12
	sw	$fp, 12($sp)
	sw	$s0, 8($sp)
	sw	$ra, 4($sp)
	addiu	$fp, $sp, 4
	move	$s0, $a0
	jal	Object_init
	move	$a0, $s0
	lw	$fp, 12($sp)
	lw	$s0, 8($sp)
	lw	$ra, 4($sp)
	addiu	$sp, $sp, 12
	jr	$ra
Main_init:
	addiu	$sp, $sp, -12
	sw	$fp, 12($sp)
	sw	$s0, 8($sp)
	sw	$ra, 4($sp)
	addiu	$fp, $sp, 4
	move	$s0, $a0
	jal	Object_init
	move	$a0, $s0
	lw	$fp, 12($sp)
	lw	$s0, 8($sp)
	lw	$ra, 4($sp)
	addiu	$sp, $sp, 12
	jr	$ra
Main.main:
	addiu	$sp, $sp, -12
	sw	$fp, 12($sp)
	sw	$s0, 8($sp)
	sw	$ra, 4($sp)
	la	$a0, int_const_1
	lw	$fp, 12($sp)
	lw	$s0, 8($sp)
	lw	$ra, 4($sp)
	addiu	$sp, $sp, 12
	jr	$ra
