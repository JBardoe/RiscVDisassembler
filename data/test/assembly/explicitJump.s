.text
.globl _start

_start:
    addi x5, x0, 1

    jal  x0, .+8

    addi x5, x5, 10
    addi x5, x5, 20

    addi x5, x5, 100

    ebreak

