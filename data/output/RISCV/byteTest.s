.text
.globl _start

_start:
	la t0, my_byte          # t0 = &my_byte
	lb t1, 0(t0)            # t1 = M[t0+0][0:7]
	addi t1, t1, 1          # t1 = t1 + 1
	sb t1, 0(t0)            # M[t0+0][0:7] = t1[0:7]
	jal zero, _start        # zero = PC+4; PC = &_start

.data
	my_byte:	.byte 16


