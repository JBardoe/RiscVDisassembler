.text
.globl _start
.globl entry_a
.globl entry_b

_start:
	adr x9, var_byte        // x9 = &var_byte
	ldrsb x10, [x9, #0]     // x10 = Memory[x9+0][0:7] (sign-extends)
	adr x9, var_half        // x9 = &var_half
	ldrsh x11, [x9, #0]     // x11 = Memory[x9+0][0:15] (sign-extends)
	adr x9, var_word        // x9 = &var_word
	ldrsw x12, [x9, #0]     // x12 = Memory[x9+0][0:31]
	add x13, x10, x11       // x13 = x10 + x11
	add x13, x13, x12       // x13 = x13 + x12
	b entry_a               // Branch to entry_a 

entry_a:
	adr x0, var_word        // x0 = &var_word
	ldrsw x1, [x0, #0]      // x1 = Memory[x0+0][0:31]
	add x1, x1, #1          // x1 = x1 + 1
	str x0, [x1, #0]        // Memory[x0+0][0:31] = x1[0:31]
	b entry_b               // Branch to entry_b 

entry_b:
	adr x0, var_byte        // x0 = &var_byte
	ldrsb x1, [x0, #0]      // x1 = Memory[x0+0][0:7] (sign-extends)
	add x1, x1, #2          // x1 = x1 + 2
	strb x0, [x1, #0]       // Memory[x0+0][0:7] = x1[0:7]
	b entry_b               // Branch to entry_b 

.data
	var_word:	.word 100000
	var_half:	.half 300
	var_byte:	.byte 5


