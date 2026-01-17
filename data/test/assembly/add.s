.data
num1: .word 3
num2: .word 8
result: .word 0

.text
.globl _start

_start:
	la t0, num1
	la t1, num2
	la t2, result

	lw t3, 0(t0)
	lw t4, 0(t1)
	add t5, t3, t4
	sw t5, 0(t2)

	li a0, 0
	li a7, 93
	ecall
