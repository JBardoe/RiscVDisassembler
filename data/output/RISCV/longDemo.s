.text
.globl _start
.local init
.local copy_outer
.local copy_inner

_start:
	la t0, src              # t0 = &src
	lui t1, 4               # t1 = 4 << 12
	addi t2, zero, 0        # t2 = zero + 0

init:
	sw t2, 0(t0)            # M[t0+0][0:31] = t2[0:31]
	addi t2, t2, 1          # t2 = t2 + 1
	addi t0, t0, 4          # t0 = t0 + 4
	addi t1, t1, -1         # t1 = t1 + -1
	bne t1, zero, init      # if(t1 != zero) PC = &init
	addi s0/fp, zero, 1000  # s0/fp = zero + 1000

copy_outer:
	la t0, src              # t0 = &src
	la t1, dst              # t1 = &dst
	lui t2, 4               # t2 = 4 << 12

copy_inner:
	lw t3, 0(t0)            # t3 = M[t0+0][0:31]
	sw t3, 0(t1)            # M[t1+0][0:31] = t3[0:31]
	addi t0, t0, 4          # t0 = t0 + 4
	addi t1, t1, 4          # t1 = t1 + 4
	addi t2, t2, -1         # t2 = t2 + -1
	bne t2, zero, copy_inner# if(t2 != zero) PC = &copy_inner
	addi s0/fp, s0/fp, -1   # s0/fp = s0/fp + -1
	bne s0/fp, zero, copy_outer# if(s0/fp != zero) PC = &copy_outer
	addi a7, zero, 93       # a7 = zero + 93
	addi a0, zero, 0        # a0 = zero + 0
	ecall                   # Transfer control to OS

.bss
	dst .space 4
	src .space 4


