.text
    .global  _start

_start:
    la      t0, src
    li      t1, 16384
    li      t2, 0

init:
    sw      t2, 0(t0)
    addi    t2, t2, 1
    addi    t0, t0, 4
    addi    t1, t1, -1
    bnez    t1, init

    li      s0, 1000

copy_outer:
    la      t0, src
    la      t1, dst
    li      t2, 16384

copy_inner:
    lw      t3, 0(t0)
    sw      t3, 0(t1)
    addi    t0, t0, 4
    addi    t1, t1, 4
    addi    t2, t2, -1
    bnez    t2, copy_inner

    addi    s0, s0, -1
    bnez    s0, copy_outer

    li      a7, 93
    li      a0, 0
    ecall

.bss
src:    .space 4
dst:    .space 4

