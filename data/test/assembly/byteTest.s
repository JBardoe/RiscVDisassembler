.text
.globl _start

_start:
    la   t0, my_byte
    lb   t1, 0(t0)
    addi t1, t1, 1
    sb   t1, 0(t0)
    j    _start

.data
my_byte:	.byte 0x10

