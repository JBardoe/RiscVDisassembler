.text
.globl _start

_start:
	la t0, __global_pointer$# t0 = &__global_pointer$
	addi a0, t0, 0          # a0 = t0 + 0
	addi a7, zero, 93       # a7 = zero + 93
	ecall                   # Transfer control to OS

.data
	__global_pointer$:	.word 71824


