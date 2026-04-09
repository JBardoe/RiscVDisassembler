.text
.globl _start

_start:
	adr x9, my_half         // x9 = &my_half
	ldrsh x10, [x9, #0]     // x10 = Memory[x9+0][0:15] (sign-extends)
	add x10, x10, #1        // x10 = x10 + 1
	strh x9, [x10, #0]      // Memory[x9+0][0:15] = x10[0:15]
	b _start                // Branch to _start 

.data
	my_half:	.half 4660


