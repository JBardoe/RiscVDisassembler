.text
.globl _start

_start:
	adr, w9, my_half        // w9 = &my_half
	ldrsh w10, [w9, #0]     // w10 = Memory[w9+0][0:15] (sign-extends)
	add w10, w10, #1        // w10 = w10 + 1
	strh w9, [w10, #0]      // Memory[w9+0][0:15] = w10[0:15]
	b _start                // Branch to _start 

.data
	my_half:	.half 4660


