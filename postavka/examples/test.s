.globl main

.data
m1: .word 6
m2: .word 5
m3: .word 0

.text
main:
	la $t1, m1
	lw $t1, 0($t1)
	la $t1, m2
	lb $t1, 0($t1)
	li $t2, 1
	li $t1, 0
lab:
	and $t1, $t1, $t1
	or $t0, $t2, $t0
	addi $t2, $t2, 1
	bltz $t0, lab
	la $t0, m3
	sw $t1, 0($t0)
	nop
