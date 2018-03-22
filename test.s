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
	add	$15,$0,4
	sw	$15,12($fp)
	lw	$8,12($sp)
	lw	$8,12($sp)
	add	$8,$0,$8
TEST
	lw	$9,12($sp)
TEST
	beq	$8,$0,$eq00
	addi	$8,$0,0
	beq	$8,$0,$skip1
$eq00:
	addi	$8,$0,1
$skip1:
	beq	$8,$0,$else_label2
	lw	$10,12($sp)
	lw	$10,12($sp)
	add	$10,$0,$10
	addi	$11,$0,3
	move	$10,$11
	sw	$10,12($sp)
	bne	$8,$0,$exit_label3
$else_label2:
	lw	$12,12($sp)
	lw	$12,12($sp)
	add	$12,$0,$12
	addi	$13,$0,4
	seq	$12,$12,$13
	beq	$12,$0,$if_label4
	lw	$14,12($sp)
	lw	$14,12($sp)
	add	$14,$0,$14
	addi	$8,$0,2
	move	$14,$8
	sw	$14,12($sp)
$if_label4:
$exit_label3:
	sw	$9,12($fp)
	lw	$9,12($sp)
	add	$2,$0,$9
	addu	$sp,$fp,$0
	lw	$31,8($sp)
	lw	$fp,4($sp)
	addiu	$sp,$sp,12
	j	$31
	nop
	lw	$15,8($fp)
	sw	$15,12($fp)
	lw	$15,4($fp)
	sw	$15,8($fp)
	addiu	$sp,$sp,4
	move	$fp,$sp
	.end main
