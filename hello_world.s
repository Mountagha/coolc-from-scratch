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
str_const8:
	.word	7
	.word	4
	.word	String_dispTab
	.word	int_const10
	.ascii	""
	.byte	0
	.align	2
	.word	-1
str_const2:
	.word	7
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"Bool"
	.byte	0
	.align	2
	.word	-1
str_const1:
	.word	7
	.word	8
	.word	String_dispTab
	.word	int_const7
	.ascii	"Hello, World.\n"
	.byte	0
	.align	2
	.word	-1
str_const3:
	.word	7
	.word	5
	.word	String_dispTab
	.word	int_const4
	.ascii	"IO"
	.byte	0
	.align	2
	.word	-1
str_const4:
	.word	7
	.word	5
	.word	String_dispTab
	.word	int_const8
	.ascii	"Int"
	.byte	0
	.align	2
	.word	-1
str_const5:
	.word	7
	.word	5
	.word	String_dispTab
	.word	int_const2
	.ascii	"Main"
	.byte	0
	.align	2
	.word	-1
str_const6:
	.word	7
	.word	6
	.word	String_dispTab
	.word	int_const9
	.ascii	"Object"
	.byte	0
	.align	2
	.word	-1
str_const7:
	.word	7
	.word	6
	.word	String_dispTab
	.word	int_const9
	.ascii	"String"
	.byte	0
	.align	2
	.word	-1
str_const0:
	.word	7
	.word	8
	.word	String_dispTab
	.word	int_const0
	.ascii	"hello_world.cl"
	.byte	0
	.align	2
	.word	-1
int_const10:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	0
	.word	-1
int_const6:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	10
	.word	-1
int_const0:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	14
	.word	-1
int_const7:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	15
	.word	-1
int_const4:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	2
	.word	-1
int_const8:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	3
	.word	-1
int_const2:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	4
	.word	-1
int_const1:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	5
	.word	-1
int_const9:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	6
	.word	-1
int_const3:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	8
	.word	-1
int_const5:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	9
	.word	-1
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
class_nameTab:
	.space	16
	.word	str_const6
	.word	str_const2
	.word	str_const4
	.word	str_const7
	.word	str_const3
	.word	str_const5
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
	.word	IO.out_string
	.word	IO.out_int
	.word	IO.in_string
	.word	IO.in_int
	.word	Main.main
Object_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
String_dispTab:
	.word	Object.abort
	.word	Object.type_name
	.word	Object.copy
	.word	String.length
	.word	String.concat
	.word	String.substr
IO_protObj:
	.word	8
	.word	3
	.word	IO_dispTab
Object_protObj:
	.word	4
	.word	3
	.word	Object_dispTab
Int_protObj:
	.word	6
	.word	4
	.word	Int_dispTab
	.word	0
Main_protObj:
	.word	9
	.word	3
	.word	Main_dispTab
String_protObj:
	.word	7
	.word	5
	.word	String_dispTab
	.word	0
	.word	0
Bool_protObj:
	.word	5
	.word	4
	.word	Bool_dispTab
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
	la	$a0, int_const10
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
	jal	IO_init
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
	move	$s0, $a0
	addiu	$sp, $sp, -16
	sw	$fp, 16($sp)
	sw	$s0, 12($sp)
	la	$a0, str_const1
	sw	$a0, 8($sp)
	move	$a0, $s0
	addiu	$fp, $sp, 4
	bne	$a0, $zero, DispatchLabel0
	la	$a0, str_const0
	li	$t1, 1
	jal	_dispatch_abort
DispatchLabel0:
	lw	$t1, 8($a0)
	lw	$t1, 12($t1)
	jalr	$t1
	lw	$fp, 12($sp)
	lw	$s0, 8($sp)
	lw	$ra, 4($sp)
	addiu	$sp, $sp, 12
	jr	$ra
