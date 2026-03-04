.text
.globl _start
.globl entry_a
.globl entry_b

_start:
	la t0, var_byte         # t0 = &var_byte
	lb t1, 0(t0)            # t1 = M[t0+0][0:7]
	la t0, var_half         # t0 = &var_half
	lh t2, 0(t0)            # t2 = M[t0+0][0:15]
	la t0, var_word         # t0 = &var_word
	lw t3, 0(t0)            # t3 = M[t0+0][0:31]
	add t4, t1, t2          # t4 = t1 + t2
	add t4, t4, t3          # t4 = t4 + t3
	jal zero, entry_a       # zero = PC+4; PC = &entry_a

entry_a:
	la a0, var_word         # a0 = &var_word
	lw a1, 0(a0)            # a1 = M[a0+0][0:31]
	addi a1, a1, 1          # a1 = a1 + 1
	sw a1, 0(a0)            # M[a0+0][0:31] = a1[0:31]
	jal zero, entry_b       # zero = PC+4; PC = &entry_b

entry_b:
	la a0, var_byte         # a0 = &var_byte
	lb a1, 0(a0)            # a1 = M[a0+0][0:7]
	addi a1, a1, 2          # a1 = a1 + 2
	sb a1, 0(a0)            # M[a0+0][0:7] = a1[0:7]
	jal zero, entry_b       # zero = PC+4; PC = &entry_b

.data
	var_word:	.word 100000
	var_half:	.half 300
	var_byte:	.byte 5


