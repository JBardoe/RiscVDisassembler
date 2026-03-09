.text
.globl _start
.globl entry_a
.globl entry_b

_start:
	addi ra, zero, 1        # ra = zero + 1
	jal zero, entry_a       # zero = PC+4; PC = &entry_a

entry_a:
	addi sp, zero, 2        # sp = zero + 2
	beq ra, sp, entry_b     # if(ra == sp) PC = &entry_b
	jal zero, entry_b       # zero = PC+4; PC = &entry_b

entry_b:
	addi gp, zero, 3        # gp = zero + 3
	jal zero, entry_b       # zero = PC+4; PC = &entry_b

