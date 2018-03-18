	.data	
	.global x
	.size	x, 4
x:
	.word	0
	.data	
	.global y
	.size	y, 4
y:
	.word	0
	.text
	.global main
	.ent main
main:
	addiu	$sp,$sp,-8
	sw	$31,8($sp)
	sw	$fp,4($sp)
	addu	$fp,$sp,$0
not global: x
	lw	$8,0($sp)
	lw	$8,0($sp)
	add	$8,$0,$8
	lw	$9,0($sp)
	lw	$9,0($sp)
	move	$8,$9
	sw	$8,0($sp)
	lw	$10,0($sp)
	lw	$11,0($sp)
	add	$2,$0,$11
	addu	$sp,$fp,$0
	lw	$31,8($sp)
	lw	$fp,4($sp)
	addiu	$sp,$sp,8
	j	$31
	nop
	lw	$15,8($fp)
	sw	$15,8($fp)
	lw	$15,4($fp)
	sw	$15,4($fp)
	addiu	$sp,$sp,0
	move	$fp,$sp
	.end main
