.data
	result:	.word 0
	num2:	.word 8
	num1:	.word 3


.text
.globl _start

_start:
	adr, w9, num1           // w9 = &num1
	adr, w10, num2          // w10 = &num2
	adr, w11, result        // w11 = &result
	ldrsw w12, [w9, #0]     // w12 = Memory[w9+0][0:31] (sign-extends)
	ldrsw w13, [w10, #0]    // w13 = Memory[w10+0][0:31] (sign-extends)
	add w14, w12, w13       // w14 = w12 + w13
	str w11, [w14, #0]      // Memory[w11+0][0:31] = w14[0:31]
	add w0, wzr, #0         // w0 = wzr + 0
	add w8, wzr, #93        // w8 = wzr + 93
	svc #0                  // Transfer control to OS

