.text
.globl _start
.globl entry_a
.globl entry_b

_start:
    la   t0, var_byte
    lb   t1, 0(t0)

    la   t0, var_half
    lh   t2, 0(t0)

    la   t0, var_word
    lw   t3, 0(t0)

    add  t4, t1, t2
    add  t4, t4, t3

    j    entry_a

entry_a:
    la   a0, var_word
    lw   a1, 0(a0)
    addi a1, a1, 1
    sw   a1, 0(a0)
    j    entry_b

entry_b:
    la   a0, var_byte
    lb   a1, 0(a0)
    addi a1, a1, 2
    sb   a1, 0(a0)
    j    entry_b

.data
var_byte:	.byte  5
var_half:	.half  300
var_word:	.word  100000

