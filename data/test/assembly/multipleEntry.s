.text
.globl _start
.globl entry_a
.globl entry_b

_start:
    addi x1, x0, 1        
    jal  x0, entry_a

entry_a:
    addi x2, x0, 2
    beq  x1, x2, entry_b 
    jal  x0, entry_b

entry_b:
    addi x7, x0, 3
    jal  x0, entry_b

