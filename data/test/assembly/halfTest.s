.text
.globl _start

_start:
    la   t0, my_half
    lh   t1, 0(t0)
    addi t1, t1, 1
    sh   t1, 0(t0)
    j    _start

.data
my_half:	.half 0x1234

