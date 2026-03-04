.text
.globl _start

_start:
	la t0, my_half          # t0 = &my_half
	lh t1, 0(t0)            # t1 = M[t0+0][0:15]
	addi t1, t1, 1          # t1 = t1 + 1
	sh t1, 0(t0)            # M[t0+0][0:15] = t1[0:15]
	jal zero, _start        # zero = PC+4; PC = &_start

.data
	my_half:	.half 4660


