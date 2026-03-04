.text
.globl _start

_start:
	la t0, num1             # t0 = &num1
	la t1, num2             # t1 = &num2
	la t2, result           # t2 = &result
	lw t3, 0(t0)            # t3 = M[t0+0][0:31]
	lw t4, 0(t1)            # t4 = M[t1+0][0:31]
	add t5, t3, t4          # t5 = t3 + t4
	sw t5, 0(t2)            # M[t2+0][0:31] = t5[0:31]
	addi a0, zero, 0        # a0 = zero + 0
	addi a7, zero, 93       # a7 = zero + 93
	ecall                   # Transfer control to OS

.data
	result:	.word 0
	num2:	.word 8
	num1:	.word 3


