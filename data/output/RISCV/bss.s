.text
.globl _start

_start:
	la t0, myVar            # t0 = &myVar
	addi t1, zero, 10       # t1 = zero + 10
	sw t1, 0(t0)            # M[t0+0][0:31] = t1[0:31]
	lw t2, 0(t0)            # t2 = M[t0+0][0:31]
	addi t2, t2, 5          # t2 = t2 + 5
	sw t2, 0(t0)            # M[t0+0][0:31] = t2[0:31]
	addi a7, zero, 93       # a7 = zero + 93
	addi a0, zero, 0        # a0 = zero + 0
	ecall                   # Transfer control to OS

.bss
	myVar .space 4


