	.text
	.global f
	.ent f
f:
	addiu	$sp,$sp,-8
	sw	$31,8($sp)
	sw	$fp,4($sp)
	addu	$fp,$sp,$0
	addi	$2,$0,4
	addu	$sp,$fp,$0
	lw	$31,8($sp)
	lw	$fp,4($sp)
	addiu	$sp,$sp,8
	j	$31
	nop
	.end f
	.text
	.global main
	.ent main
main:
	addiu	$sp,$sp,-8
	sw	$31,8($sp)
	sw	$fp,4($sp)
	addu	$fp,$sp,$0
	addiu	$sp,$sp,-4
	lw	$15,8($sp)
	sw	$15,4($sp)
	lw	$15,12($sp)
	sw	$15,8($sp)
	addu	$fp,$sp,$0
	add	$15,$0,2
	sw	$15,12($fp)
	lw	$8,12($sp)
	lw	$8,12($sp)
	add	$8,$0,$8
	lw	$9,27876($sp)
	lw	$9,27876($sp)
	add	$9,$0,$9
	addiu	$sp,$sp,-4
	lw	$15,8($sp)
	sw	$15,4($sp)
	lw	$15,12($sp)
	sw	$15,8($sp)
	lw	$15,16($sp)
	sw	$15,12($sp)
	addu	$fp,$sp,$0
	jal	f
nop
	sw	$2,4($fp)
	move	$9,$2
	move	$8,$9
	sw	$8,12($sp)
	lw	$10,12($sp)
	add	$2,$0,$10
	addu	$sp,$fp,$0
	lw	$31,8($sp)
	lw	$fp,4($sp)
	addiu	$sp,$sp,16
	j	$31
	nop
	.end main
