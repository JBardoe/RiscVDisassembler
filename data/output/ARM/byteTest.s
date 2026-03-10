.text
.globl _start

_start:
	adr, w9, my_byte        // w9 = &my_byte
	ldrsb w10, [w9, #0]     // w10 = Memory[w9+0][0:7] (sign-extends)
	add w10, w10, #1        // w10 = w10 + 1
	strb w9, [w10, #0]      // Memory[w9+0][0:7] = w10[0:7]
	b _start                // Branch to _start 

.data
	my_byte:	.byte 16


