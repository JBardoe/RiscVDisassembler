.text
.globl _start

_start:
	adr x9, my_byte         // x9 = &my_byte
	ldrsb x10, [x9, #0]     // x10 = Memory[x9+0][0:7] (sign-extends)
	add x10, x10, #1        // x10 = x10 + 1
	strb x9, [x10, #0]      // Memory[x9+0][0:7] = x10[0:7]
	b _start                // Branch to _start 

.data
	my_byte:	.byte 16


