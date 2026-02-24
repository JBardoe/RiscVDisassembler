.bss
myVar: .space 4

.text
.globl _start

_start:
    la t0, myVar      

    li t1, 10
    sw t1, 0(t0)

    lw t2, 0(t0)
    addi t2, t2, 5
    sw t2, 0(t0)

    li a7, 93
    li a0, 0
    ecall

