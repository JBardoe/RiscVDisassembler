.text
.globl _start

_start:
	addi t0, gp, 0
	addi a0, t0, 0
	li a7, 93
	ecall

