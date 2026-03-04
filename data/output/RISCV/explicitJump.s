.text
.globl _start
.local L.diss.0

_start:
	addi t0, zero, 1        # t0 = zero + 1
	jal zero, L.diss.0      # zero = PC+4; PC = &L.diss.0
	addi t0, t0, 10         # t0 = t0 + 10

L.diss.0:
	addi t0, t0, 20         # t0 = t0 + 20
	addi t0, t0, 100        # t0 = t0 + 100
	ebreak                  # Transfer control to debugger

