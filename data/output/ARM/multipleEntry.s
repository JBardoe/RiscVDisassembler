.text
.globl _start
.globl entry_a
.globl entry_b

_start:
	add lr, xzr, #1         // lr = xzr + 1
	b entry_a               // Branch to entry_a 

entry_a:
	add sp, xzr, #2         // sp = xzr + 2
	cmp lr, sp              // set flags for lr - sp
	b.eq entry_b            // Branch to entry_b if most_recent_cmp was equal
	b entry_b               // Branch to entry_b 

entry_b:
	add x11, xzr, #3        // x11 = xzr + 3
	b entry_b               // Branch to entry_b 

